#pragma once

#include <QObject>

#include "dmxcontroller.h"

class PresetModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(int presetId READ presetId WRITE setPresetId NOTIFY presetIdChanged)

public:
    using QObject::QObject;

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    int presetId() const { return m_presetId; }
    void setPresetId(int presetId);

    Q_INVOKABLE void copyFromFaders();
    Q_INVOKABLE void copyToFaders();
    Q_INVOKABLE void setAllFadersLow();
    Q_INVOKABLE void setAllFadersMax();
    Q_INVOKABLE void setPattern(int n, int k, DeviceTypeRegisterType registerType, quint8 value);

signals:
    void controllerChanged(DmxController *controller);
    void presetIdChanged(int presetId);

private:
    DmxController *m_controller{};
    int m_presetId{-1};
};
