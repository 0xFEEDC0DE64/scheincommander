#include "dmxcontroller.h"

//#include <unistd.h>
//#include <sys/ioctl.h>

#include <QDebug>
#include <QFile>
#include <QMutexLocker>
#include <QElapsedTimer>

#include "projectloader.h"

DmxController::DmxController(ScheinCommanderSettings &settings, QObject *parent) :
    QObject{parent},
    m_settings{settings},
    m_thread{*this},
    m_lastInfo{QDateTime::currentDateTime()}
{
}

bool DmxController::start()
{
    m_serialPort.setPortName("/dev/ttyAMA0");
    if (!m_serialPort.setBaudRate(250000))
    {
        qCritical("setBaudRate() failed %s", qPrintable(m_serialPort.errorString()));
        return false;
    }
    if (!m_serialPort.setDataBits(QSerialPort::Data8))
    {
        qCritical("setDataBits() failed %s", qPrintable(m_serialPort.errorString()));
        return false;
    }
    if (!m_serialPort.setParity(QSerialPort::NoParity))
    {
        qCritical("setParity() failed %s", qPrintable(m_serialPort.errorString()));
        return false;
    }
    if (!m_serialPort.setStopBits(QSerialPort::TwoStop))
    {
        qCritical("setStopBits() failed %s", qPrintable(m_serialPort.errorString()));
        return false;
    }
    if (!m_serialPort.setFlowControl(QSerialPort::NoFlowControl))
    {
        qCritical("setFlowControl() failed %s", qPrintable(m_serialPort.errorString()));
        return false;
    }
    if (!m_serialPort.open(QIODevice::WriteOnly))
    {
        qCritical("open() failed %s", qPrintable(m_serialPort.errorString()));
        return false;
    }

    m_thread.start(QThread::TimeCriticalPriority);

    return true;
}

bool DmxController::loadProject(const QString &name)
{
    QFile file{name};
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Error opening project file: " << file.errorString();
        return false;
    }

    QByteArray json = file.readAll();
    if (json.size() == 0)
    {
        qDebug() << "Error reading project file";
        return false;
    }

    QJsonParseError error;
    QJsonDocument jd = QJsonDocument::fromJson(json, &error);
    if (jd.isNull())
    {
        qDebug() << "Error parsing JSON: " << error.errorString();
        return false;
    }

    if (auto proj = ProjectLoader::loadProject(jd); proj)
    {
        m_lightProject = proj.value();
    }
    else
    {
        qFatal("loading project failed: %s", qPrintable(proj.error()));
        return false;
    }

    return true;
}

bool DmxController::loadProject(const QUrl &url)
{
    return loadProject(url.toLocalFile());
}

bool DmxController::saveProject(const QString &name)
{
    QFile file{name};
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Error opening file: " << file.errorString();
        return false;
    }

    auto proj = ProjectLoader::saveProject(m_lightProject);
    if (!proj)
    {
        qDebug() << proj.error();
        return false;
    }

    QByteArray json = proj.value().toJson();
    if (file.write(json) != json.size())
    {
        qDebug() << "Error writing file: " << file.errorString();
        return false;
    }

    m_settings.setLastProjectFile(name);

    return true;
}

bool DmxController::saveProject(const QUrl &url)
{
    return saveProject(url.toLocalFile());
}

void DmxController::setPresetSlider(int presetId, quint8 value)
{
    const auto presetPtr = m_lightProject.presets.findById(presetId);
    if (!presetPtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = presetPtr - &*std::cbegin(m_lightProject.presets);

    {
        QMutexLocker locker{&m_mutex};

        if (index >= m_presetStates.size())
            m_presetStates.resize(index + 1);

        m_presetStates[index] = value;
    }
}

void DmxController::setSliderStates(sliders_state_t &&sliderStates)
{
    {
        QMutexLocker locker{&m_mutex};
        m_sliderStates = std::move(sliderStates);
    }
    emit sliderStatesChanged(m_sliderStates);
}

void DmxController::setSliderStates(const sliders_state_t &sliderStates)
{
    {
        QMutexLocker locker{&m_mutex};
        m_sliderStates = sliderStates;
    }
    emit sliderStatesChanged(m_sliderStates);
}

void DmxController::sendDmxBuffer()
{
    const auto now = QDateTime::currentDateTime();
    const auto secsSinceEpoch = now.toSecsSinceEpoch();

    char buf[513] {0};

    QElapsedTimer timer;
    timer.start();

    {
        QMutexLocker locker{&m_mutex};

        const auto apply = [&](const sliders_state_t &sliders, quint8 factor){
            auto iter = std::cbegin(sliders);

            for (const auto &light : m_lightProject.devices)
            {
                if (!light.address)
                {
                    if (iter != std::cend(sliders))
                        iter++;
                    continue;
                }

                auto deviceTypePtr = m_lightProject.deviceTypes.findById(light.deviceTypeId);
                if (!deviceTypePtr)
                {
                    if (iter != std::cend(sliders))
                        iter++;
                    continue;
                }
                const auto &deviceType = *deviceTypePtr;

                {
                    std::vector<quint8>::const_iterator iter2;
                    if (iter != std::cend(sliders))
                        iter2 = std::cbegin(*iter);

                    int i{};
                    for (const auto &register_ : deviceType.registers)
                    {
                        const auto address = light.address + (i++);

                        if (iter != std::cend(sliders) && iter2 != std::cend(*iter))
                        {
                            const auto result = int(*iter2) * factor / 255;
                            if (result > buf[address])
                                buf[address] = result;
                        }
//                          else
//                              buf[address] = 0;

                        if (iter != std::cend(sliders) && iter2 != std::cend(*iter))
                            iter2++;
                    }
                }

                if (iter != std::cend(sliders))
                    iter++;
            }
        };

        apply(m_sliderStates, 255);

        auto iter = std::cbegin(m_presetStates);
        for (const auto &preset : m_lightProject.presets)
        {
            if (iter == std::cend(m_presetStates))
                break;
            if (!*iter)
            {
                iter++;
                continue;
            }
            if (preset.steps.empty())
            {
                iter++;
                continue;
            }

            const auto &sliders = preset.steps[secsSinceEpoch % preset.steps.size()].sliders;
            apply(sliders, *iter);
            iter++;
        }
    }

    if (const auto elapsed = timer.elapsed(); elapsed > m_computeMaxElapsed)
        m_computeMaxElapsed = elapsed;

    timer.restart();

    m_serialPort.setBreakEnabled(true);
    QThread::usleep(88);
    m_serialPort.setBreakEnabled(false);

//    const auto written = write(m_serialPort.handle(), buf, std::size(buf));
    const auto written = m_serialPort.write(buf, std::size(buf));
    m_serialPort.flush();
//    qDebug("%lli written", written);

    if (const auto elapsed = timer.elapsed(); elapsed > m_dmxMaxElapsed)
        m_dmxMaxElapsed = elapsed;

    m_counter++;

    if (m_lastInfo.msecsTo(now) >= 1000)
    {
        qInfo("%i per second", m_counter);

        m_lastInfo = now;

        m_lastCounter = m_counter;
        emit dmxFpsChanged(m_counter);
        m_counter = 0;

        m_lastDmxMaxElapsed = m_dmxMaxElapsed;
        emit dmxMaxElapsedChanged(m_lastDmxMaxElapsed);
        m_dmxMaxElapsed = 0;

        m_lastComputeMaxElapsed = m_computeMaxElapsed;
        emit computeMaxElapsedChanged(m_lastComputeMaxElapsed);
        m_computeMaxElapsed = 0;
    }
}
