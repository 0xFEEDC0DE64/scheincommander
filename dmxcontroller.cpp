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
                .name="RGB Strahler"
            },
            {
                .id=3,
                .name="Nebelmaschine"
            }
        },
        .devices {
            { .id=0,  .name="Lampe 1",       .deviceTypeId=1, .address=32, .position{1,0,0} },
            { .id=1,  .name="Lampe 2",       .deviceTypeId=1, .address=0, .position{2,0,0}  },
            { .id=2,  .name="Lampe 3",       .deviceTypeId=1, .address=7,  .position{3,0,0}  },
            { .id=3,  .name="Lampe 4",       .deviceTypeId=1, .address=14 },
            { .id=4,  .name="Moving Head 1", .deviceTypeId=0, .address=40 },
            { .id=5,  .name="Moving Head 2", .deviceTypeId=0, .address=43 },
            { .id=6,  .name="Moving Head 3", .deviceTypeId=0, .address=46 },
            { .id=7,  .name="Moving Head 4", .deviceTypeId=0, .address=49 },
            { .id=8,  .name="Test 1",        .deviceTypeId=2, .address=70 },
            { .id=9,  .name="Test 2",        .deviceTypeId=2, .address=72 },
            { .id=10, .name="Nebelmaschine", .deviceTypeId=3, .address=80 }
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
