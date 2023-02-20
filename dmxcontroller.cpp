#include "dmxcontroller.h"

#include <unistd.h>
#include <sys/ioctl.h>

#include <QDebug>
#include <QMutexLocker>

DmxController::DmxController(QObject *parent) :
    QObject{parent},
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
            { .id=0,  .name="Lampe 1",       .deviceTypeId=4, .address=1  },
            { .id=1,  .name="Lampe 2",       .deviceTypeId=4, .address=8  },
            { .id=2,  .name="Lampe 3",       .deviceTypeId=4, .address=15 },
            { .id=3,  .name="Lampe 4",       .deviceTypeId=4, .address=22 },
            { .id=4,  .name="Lampe 5",       .deviceTypeId=4, .address=29 },
            { .id=5,  .name="Lampe 6",       .deviceTypeId=4, .address=36 },
            { .id=6,  .name="Lampe 7",       .deviceTypeId=4, .address=43 },
            { .id=7,  .name="Lampe 8",       .deviceTypeId=4, .address=50 },
            { .id=8,  .name="Lampe 9",       .deviceTypeId=4, .address=57 },
            { .id=9,  .name="Lampe 10",      .deviceTypeId=4, .address=64 },
            { .id=10, .name="Lampe 11",      .deviceTypeId=4, .address=71 },
            { .id=11, .name="Lampe 12",      .deviceTypeId=4, .address=78 },
            { .id=12, .name="Lampe 13",      .deviceTypeId=4, .address=85 },
            { .id=13, .name="Test 1",        .deviceTypeId=1, .address=95 },
            { .id=14, .name="Test 2",        .deviceTypeId=2, .address=105 },
            { .id=15, .name="Moving Head 1", .deviceTypeId=0, .address=115 },
            { .id=16, .name="Moving Head 2", .deviceTypeId=0, .address=131 },
            { .id=17, .name="Moving Head 3", .deviceTypeId=0, .address=147 },
            { .id=18, .name="Moving Head 4", .deviceTypeId=0, .address=163 },
            { .id=19, .name="Nebelmaschine", .deviceTypeId=3, .address=179 }
        },
        .registerGroups {
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

void DmxController::setRegisterGroup(int registerGroupId, quint8 value)
{
    qDebug() << registerGroupId << value;
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

    {
        QMutexLocker locker{&m_mutex};

//        if (m_sliderStates.size() < m_lightProject.devices.size())
//            m_sliderStates.resize(m_lightProject.devices.size());

        auto iter = std::cbegin(m_sliderStates);

        for (const auto &light : m_lightProject.devices)
        {
            auto deviceTypePtr = m_lightProject.deviceTypes.findById(light.deviceTypeId);
            if (!deviceTypePtr)
            {
                if (iter != std::cend(m_sliderStates))
                    iter++;
                continue;
            }
            const auto &deviceType = *deviceTypePtr;

            std::vector<int>::const_iterator iter2;
            if (iter != std::cend(m_sliderStates))
                iter2 = std::cbegin(*iter);

            int i{};
            for (const auto &register_ : deviceType.registers)
            {
                const auto address = light.address + (i++);

                if (iter != std::cend(m_sliderStates) && iter2 != std::cend(*iter))
                {
                    buf[address] = *iter2;
                }
                else
                    buf[address] = 0;

                if (iter != std::cend(m_sliderStates) && iter2 != std::cend(*iter))
                    iter2++;
            }

            if (iter != std::cend(m_sliderStates))
                iter++;
        }
    }

    m_serialPort.setBreakEnabled(true);
    QThread::usleep(88);
    m_serialPort.setBreakEnabled(false);

//    const auto written = write(m_serialPort.handle(), buf, std::size(buf));
    const auto written = m_serialPort.write(buf, std::size(buf));
    m_serialPort.flush();
//    qDebug("%lli written", written);

    m_counter++;

    if (m_lastInfo.msecsTo(now) >= 1000)
    {
        qInfo("%i per second", m_counter);
        m_lastCounter = m_counter;
        emit performanceChanged(m_counter);
        m_counter = 0;
        m_lastInfo = now;
    }
}
