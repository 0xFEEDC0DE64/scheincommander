#pragma once

#include <QObject>

#include "dmxcontroller.h"

class RegisterGroupModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(int registerGroupId READ registerGroupId WRITE setRegisterGroupId NOTIFY registerGroupIdChanged)

public:
    using QObject::QObject;

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    int registerGroupId() const { return m_registerGroupId; }
    void setRegisterGroupId(int registerGroupId);

    Q_INVOKABLE void copyFromFaders();
    Q_INVOKABLE void copyToFaders();
    Q_INVOKABLE void setAllFadersLow();
    Q_INVOKABLE void setAllFadersMax();
    Q_INVOKABLE void setPattern(int n, int k, DeviceTypeRegisterType registerType, quint8 value);

signals:
    void controllerChanged(DmxController *controller);
    void registerGroupIdChanged(int registerGroupId);

private:
    DmxController *m_controller{};
    int m_registerGroupId{-1};
};
