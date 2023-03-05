#pragma once

#include <QObject>
#include <qqml.h>

#include "dmxcontroller.h"

class PatternMaker : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)

public:
    using QObject::QObject;

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    Q_INVOKABLE void setAllFaders(quint8 value);
    Q_INVOKABLE void setPattern(int n, int k, DeviceTypeRegisterType registerType, quint8 value);
    Q_INVOKABLE void setRainbow(float phase, float advancePerLamp, int advanceEveryNLamp);

signals:
    void controllerChanged(DmxController *controller);

private:
    DmxController *m_controller{};
};
