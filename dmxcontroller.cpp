#include "dmxcontroller.h"

#include <unistd.h>
#include <sys/ioctl.h>

#include <QDebug>

DmxController::DmxController(QObject *parent) :
    QObject{parent},
    m_thread{*this},
    m_lastInfo{QDateTime::currentDateTime()},
    m_counter{},
    m_lightProject {
        .deviceTypes {
            {
                .id=0,
                .name="Stairville MH-X50+"
            },
            {
                .id=1,
                .name="RGBW Strahler",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Dimmer },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Red },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Green },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Blue },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::White },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Strobo },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Dummy }
                }
            },
            {
                .id=2,
                .name="RGB Strahler",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Red },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Green },
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Blue }
                }
            },
            {
                .id=3,
                .name="Nebelmaschine",
                .registers {
                    DeviceTypeRegisterConfig { .type = DeviceTypeRegisterType::Dimmer }
                }
            }
        },
        .devices {
            { .id=0,  .name="Test 1",        .deviceTypeId=1, .address=32, .position{1,0,0} },
            { .id=1,  .name="Lampe 1",       .deviceTypeId=2, .address=1,  .position{2,0,0}  },
            { .id=1,  .name="Lampe 2",       .deviceTypeId=2, .address=4,  .position{2,0,0}  },
            { .id=2,  .name="Lampe 3",       .deviceTypeId=2, .address=7,  .position{3,0,0}  },
            { .id=3,  .name="Lampe 4",       .deviceTypeId=2, .address=10 },
            { .id=4,  .name="Lampe 5",       .deviceTypeId=2, .address=13 },
            { .id=5,  .name="Lampe 6",       .deviceTypeId=2, .address=16 },
            { .id=6,  .name="Lampe 7",       .deviceTypeId=2, .address=19 },
            { .id=7,  .name="Lampe 8",       .deviceTypeId=2, .address=22 },
            { .id=8,  .name="Lampe 9",       .deviceTypeId=2, .address=25 },
            { .id=9,  .name="Lampe 10",      .deviceTypeId=2, .address=28 },
            { .id=1,  .name="Lampe 11",      .deviceTypeId=2, .address=31 },
            { .id=1,  .name="Lampe 12",      .deviceTypeId=2, .address=34 },
            { .id=2,  .name="Lampe 13",      .deviceTypeId=2, .address=37 },
            { .id=3,  .name="Lampe 14",      .deviceTypeId=2, .address=40 },
            { .id=4,  .name="Lampe 15",      .deviceTypeId=2, .address=43 },
            { .id=5,  .name="Lampe 16",      .deviceTypeId=2, .address=46 },
            { .id=6,  .name="Lampe 17",      .deviceTypeId=2, .address=49 },
            { .id=7,  .name="Lampe 18",      .deviceTypeId=2, .address=52 },
            { .id=8,  .name="Lampe 19",      .deviceTypeId=2, .address=55 },
            { .id=9,  .name="Lampe 20",      .deviceTypeId=2, .address=58 },
            { .id=10, .name="Nebelmaschine", .deviceTypeId=3, .address=90 },
            { .id=11, .name="Moving Head 1", .deviceTypeId=0, .address=40 },
            { .id=12, .name="Moving Head 2", .deviceTypeId=0, .address=43 },
            { .id=13, .name="Moving Head 3", .deviceTypeId=0, .address=46 },
            { .id=14, .name="Moving Head 4", .deviceTypeId=0, .address=49 }
        }
    }
{
    std::fill(std::begin(buf), std::end(buf), 0);

    buf[32] = 255;
    buf[33] = 255;
    buf[34] = 0;
    buf[35] = 0;
//    buf[36] = 255;
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

void DmxController::setChannel(int channel, int value)
{
    //qDebug() << channel << value;
    Q_ASSERT(channel >= 0 && channel < std::size(buf));
    buf[channel] = value;
}

void DmxController::sendDmxBuffer()
{
    const auto now = QDateTime::currentDateTime();

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
        m_counter = 0;
        m_lastInfo = now;
    }
}
