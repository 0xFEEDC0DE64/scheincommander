#pragma once

#include <QObject>
#include <qqml.h>

#include "dmxcontroller.h"

class DeviceRegisterValueHelper : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(int deviceId READ deviceId WRITE setDeviceId NOTIFY deviceIdChanged)
    Q_PROPERTY(int registerIndex READ registerIndex WRITE setRegisterIndex NOTIFY registerIndexChanged)
    Q_PROPERTY(quint8 value READ value WRITE setValue NOTIFY valueChanged)

public:
    using QObject::QObject;

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    int deviceId() const { return m_deviceId; }
    void setDeviceId(int deviceId);

    int registerIndex() const { return m_registerIndex; }
    void setRegisterIndex(int registerIndex);

    quint8 value() const;
    void setValue(quint8 value);

signals:
    void controllerChanged(DmxController *controller);
    void deviceIdChanged(int deviceId);
    void registerIndexChanged(int registerIndex);
    void valueChanged(quint8 registerIndex);

private slots:
    void sliderStatesChanged();

private:
    DmxController *m_controller{};
    int m_deviceId{-1};
    int m_registerIndex{-1};
};
