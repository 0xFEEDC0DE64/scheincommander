#include "dmxcontroller.h"

//#include <unistd.h>
//#include <sys/ioctl.h>

#include <QDebug>
#include <QFile>
#include <QMutexLocker>
#include <QElapsedTimer>

#include "projectloader.h"

void doFun(auto &y)
{
    return;
    double i{-10};
    for (auto &x : y.devices)
    {
        x.position.setX(i);
        x.position.setZ(std::sin(i / 2.) * 10.);
        i += 20./double(y.devices.size());
    }
}

DmxController::DmxController(ScheinCommanderSettings &settings, QObject *parent) :
    QObject{parent},
    m_settings{settings},
    m_thread{*this},
    m_lastInfo{QDateTime::currentDateTime()},
    m_counter{},
    m_lightProject {
        .deviceTypes {
            {
                .id=0,
                .name="Stairville MH-X50+",
                .iconName="movinghead",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Pan },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Tilt },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::PanFine },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::TiltFine },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Speed },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Color },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Shutter },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Dimmer },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Gobo },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Rotation },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Extra1 },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Extra2 },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Prism },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Focus }
                }
            },
            {
                .id=1,
                .name="RGBW Strahler Klein",
                .iconName="rgbstrahler",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Dimmer },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Red },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Green },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Blue },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::White },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Shutter },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Extra1 }
                }
            },
            {
                .id=2,
                .name="RGB Strahler",
                .iconName="rgbstrahler",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Red },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Green },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Blue }
                }
            },
            {
                .id=3,
                .name="Nebelmaschine",
                .iconName="nebelmaschine",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Dimmer }
                }
            },
            {
                .id=4,
                .name="RGBW Strahler Groß",
                .iconName="rgbstrahler",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Dimmer },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Red },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Green },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Blue },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::White },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Extra1 },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Shutter }
                }
            }
        },
        .devices {
            { .id=0,   .name="Lampe 1",        .deviceTypeId=4, .address=1   },
            { .id=1,   .name="Lampe 2",        .deviceTypeId=4, .address=8   },
            { .id=2,   .name="Lampe 3",        .deviceTypeId=4, .address=15  },
            { .id=3,   .name="Lampe 4",        .deviceTypeId=4, .address=22  },
            { .id=4,   .name="Lampe 5",        .deviceTypeId=4, .address=29  },
            { .id=5,   .name="Lampe 6",        .deviceTypeId=4, .address=36  },
            { .id=6,   .name="Lampe 7",        .deviceTypeId=4, .address=43  },
            { .id=7,   .name="Lampe 8",        .deviceTypeId=4, .address=50  },
            { .id=8,   .name="Lampe 9",        .deviceTypeId=4, .address=57  },
            { .id=9,   .name="Lampe 10",       .deviceTypeId=4, .address=64  },
            { .id=10,  .name="Lampe 11",       .deviceTypeId=4, .address=71  },
            { .id=11,  .name="Lampe 12",       .deviceTypeId=4, .address=78  },
            { .id=12,  .name="Lampe 13",       .deviceTypeId=4, .address=85  },
            { .id=13,  .name="Lampe 14",       .deviceTypeId=4, .address=92  },
            { .id=14,  .name="Lampe 15",       .deviceTypeId=4, .address=99  },
            { .id=15,  .name="Lampe 16",       .deviceTypeId=4, .address=106 },
            { .id=16,  .name="Lampe 17",       .deviceTypeId=4, .address=113 },
            { .id=17,  .name="Lampe 18",       .deviceTypeId=4, .address=120 },
            { .id=18,  .name="Lampe 19",       .deviceTypeId=4, .address=127 },
            { .id=19,  .name="Lampe 20",       .deviceTypeId=4, .address=134 },
            { .id=20,  .name="Lampe 21",       .deviceTypeId=4, .address=141 },
            { .id=21,  .name="Lampe 22",       .deviceTypeId=4, .address=148 },
            { .id=22,  .name="Lampe 23",       .deviceTypeId=4, .address=155 },
            { .id=23,  .name="Lampe 24",       .deviceTypeId=4, .address=162 },
            { .id=24,  .name="Lampe 25",       .deviceTypeId=4, .address=169 },
            { .id=25,  .name="Lampe 26",       .deviceTypeId=4, .address=176 },
            { .id=26,  .name="Lampe 27",       .deviceTypeId=4, .address=183 },
            { .id=27,  .name="Lampe 28",       .deviceTypeId=4, .address=190 },
            { .id=28,  .name="Lampe 29",       .deviceTypeId=4, .address=197 },
            { .id=29,  .name="Lampe 30",       .deviceTypeId=4, .address=204 },
            { .id=30,  .name="Lampe 31",       .deviceTypeId=4, .address=211 },
            { .id=31,  .name="Lampe 32",       .deviceTypeId=4, .address=218 },
            { .id=32,  .name="Lampe 33",       .deviceTypeId=4, .address=225 },
            { .id=33,  .name="Lampe 34",       .deviceTypeId=4, .address=232 },
            { .id=34,  .name="Lampe 35",       .deviceTypeId=4, .address=239 },
            { .id=35,  .name="Lampe 36",       .deviceTypeId=4, .address=246 },
            { .id=36,  .name="Lampe 37",       .deviceTypeId=4, .address=253 },
            { .id=37,  .name="Lampe 38",       .deviceTypeId=4, .address=260 },
            { .id=38,  .name="Lampe 39",       .deviceTypeId=4, .address=267 },
            { .id=39,  .name="Lampe 40",       .deviceTypeId=4, .address=274 },
            { .id=40,  .name="Lampe 41",       .deviceTypeId=4, .address=281 },
            { .id=41,  .name="Lampe 42",       .deviceTypeId=4, .address=288 },
            { .id=42,  .name="Lampe 43",       .deviceTypeId=4, .address=295 },
            { .id=43,  .name="Lampe 44",       .deviceTypeId=4, .address=302 },
            { .id=44,  .name="Lampe 45",       .deviceTypeId=4, .address=309 },
            { .id=45,  .name="Lampe 46",       .deviceTypeId=4, .address=316 },
            { .id=46,  .name="Lampe 47",       .deviceTypeId=4, .address=323 },
            { .id=47,  .name="Lampe 48",       .deviceTypeId=4, .address=330 },
            { .id=48,  .name="Lampe 49",       .deviceTypeId=4, .address=337 },
            { .id=49,  .name="Lampe 50",       .deviceTypeId=4, .address=344 },
            { .id=50,  .name="Lampe 51",       .deviceTypeId=4, .address=351 },
            { .id=51,  .name="Lampe 52",       .deviceTypeId=4, .address=358 },
//            { .id=13, .name="Test 1",        .deviceTypeId=1, .address=95 },
//            { .id=14, .name="Test 2",        .deviceTypeId=2, .address=105 },
//            { .id=15, .name="Moving Head 1", .deviceTypeId=0, .address=115 },
//            { .id=16, .name="Moving Head 2", .deviceTypeId=0, .address=131 },
//            { .id=17, .name="Moving Head 3", .deviceTypeId=0, .address=147 },
//            { .id=18, .name="Moving Head 4", .deviceTypeId=0, .address=163 },
//            { .id=19, .name="Nebelmaschine", .deviceTypeId=3, .address=179 }
        },
        .presets {
            { .id=0,  .name="Alle Dimmer" },
            { .id=1,  .name="Alle Roten"   },
            { .id=2,  .name="Alle Grünen"  },
            { .id=3,  .name="Alle Blauen"  },
            { .id=4,  .name="Alle Weißen"  },
            { .id=5,  .name="Alle Shutter" },
            { .id=6,  .name="2n Dimmer"   },
            { .id=7,  .name="2n Roten"     },
            { .id=8,  .name="2n Grünen"    },
            { .id=9,  .name="2n Blauen"    },
            { .id=10, .name="2n Weißen"    },
            { .id=11, .name="2n Shutter"   },
            { .id=12, .name="2n+1 Dimmer" },
            { .id=13, .name="2n+1 Roten"   },
            { .id=14, .name="2n+1 Grünen"  },
            { .id=15, .name="2n+1 Blauen"  },
            { .id=16, .name="2n+1 Weißen"  },
            { .id=17, .name="2n+1 Shutter" },
        }
    }
{
    doFun(m_lightProject);
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
        qDebug() << proj.error();
        return false;
    }

    doFun(m_lightProject);

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
            apply(preset.sliders, *iter);
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
