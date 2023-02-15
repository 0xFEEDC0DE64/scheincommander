#pragma once

#include <QObject>
#include <QSerialPort>
#include <QDateTime>
#include <QReadWriteLock>

#include "dmxcontrollerthread.h"
#include "lightproject.h"

class DmxController : public QObject
{
    Q_OBJECT

public:
    explicit DmxController(QObject *parent = nullptr);

    bool start();

    Q_INVOKABLE void setChannel(int channel, int value);

    LightProject &lightProject() { return m_lightProject; }
    const LightProject &lightProject() const { return m_lightProject; }

    QReadWriteLock &projectLock() { return m_projectLock; }

protected:
    friend class DmxControllerThread;

    void sendDmxBuffer(); // runs in its own thread

private:
    QSerialPort m_serialPort;

    DmxControllerThread m_thread;

    char buf[513];

    LightProject m_lightProject;
    QReadWriteLock m_projectLock;

    QDateTime m_lastInfo;
    int m_counter;
};
