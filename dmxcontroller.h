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
    Q_PROPERTY(int performance READ performance NOTIFY performanceChanged)

public:
    explicit DmxController(QObject *parent = nullptr);

    bool start();

    Q_INVOKABLE void setChannel(int channel, int value);

    LightProject &lightProject() { return m_lightProject; }
    const LightProject &lightProject() const { return m_lightProject; }

    QReadWriteLock &projectLock() { return m_projectLock; }

    int performance() const { return m_lastCounter; }

signals:
    void performanceChanged(int performance);

    void deviceTypeInserted(int first, int last);
    void deviceTypeRemoved(int first, int last);
    void deviceTypeNameChanged(int index, const QString &name);
    void deviceTypeIconNameChanged(int index, const QString &iconName);

    void deviceTypeRegisterInserted(const DeviceTypeConfig &deviceType, int first, int last);
    void deviceTypeRegisterRemoved(const DeviceTypeConfig &deviceType, int first, int last);
    void deviceTypeRegisterTypeChanged(const DeviceTypeConfig &deviceType, int index, DeviceTypeRegisterType type);

    void deviceInserted(int first, int last);
    void deviceRemoved(int first, int last);
    void deviceNameChanged(int index, const QString &name);
    void deviceDeviceTypeIdChanged(int index, int deviceTypeId);
    void deviceAddressChanged(int index, int address);
    void devicePositionChanged(int index, const QVector3D &position);

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
    std::atomic<int> m_lastCounter;
};
