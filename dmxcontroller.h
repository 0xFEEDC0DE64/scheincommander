#pragma once

#include <QObject>
#include <QSerialPort>
#include <QDateTime>
#include <QMutex>

#include "dmxcontrollerthread.h"
#include "lightproject.h"

class DmxController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int performance READ performance NOTIFY performanceChanged)

public:
    explicit DmxController(QObject *parent = nullptr);

    bool start();

    Q_INVOKABLE void setRegisterGroupSlider(int registerGroupId, quint8 value);

    LightProject &lightProject() { return m_lightProject; }
    const LightProject &lightProject() const { return m_lightProject; }

    QMutex &mutex() { return m_mutex; }

    sliders_state_t &sliderStates() { return m_sliderStates; }
    const sliders_state_t &sliderStates() const { return m_sliderStates; }
    void setSliderStates(sliders_state_t &&sliderStates);
    void setSliderStates(const sliders_state_t &sliderStates);

    int performance() const { return m_lastCounter; }

signals:
    void performanceChanged(int performance);

    void deviceTypeInserted(int first, int last);
    void deviceTypeRemoved(int first, int last);
    void deviceTypeNameChanged(int row, const QString &name);
    void deviceTypeIconNameChanged(int row, const QString &iconName);

    void deviceTypeRegisterInserted(const DeviceTypeConfig &deviceType, int first, int last);
    void deviceTypeRegisterRemoved(const DeviceTypeConfig &deviceType, int first, int last);
    void deviceTypeRegisterTypeChanged(const DeviceTypeConfig &deviceType, int row, DeviceTypeRegisterType type);

    void deviceInserted(int first, int last);
    void deviceRemoved(int first, int last);
    void deviceNameChanged(int row, const QString &name);
    void deviceDeviceTypeIdChanged(int row, int deviceTypeId);
    void deviceAddressChanged(int row, int address);
    void devicePositionChanged(int row, const QVector3D &position);

    void registerGroupInserted(int first, int last);
    void registerGroupRemoved(int first, int last);
    void registerGroupNameChanged(int row, const QString &name);

    void sliderStatesChanged(const sliders_state_t &sliderStates);

protected:
    friend class DmxControllerThread;

    void sendDmxBuffer(); // runs in its own thread

private:
    QSerialPort m_serialPort;

    DmxControllerThread m_thread;

    LightProject m_lightProject;
    QMutex m_mutex;
    sliders_state_t m_sliderStates;
    std::vector<quint8> m_registerGroupStates;

    QDateTime m_lastInfo;
    int m_counter;
    std::atomic<int> m_lastCounter;
};
