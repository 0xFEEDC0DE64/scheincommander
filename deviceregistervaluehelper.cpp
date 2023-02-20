#include "deviceregistervaluehelper.h"

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QMutexLocker>

void DeviceRegisterValueHelper::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    if (m_controller)
    {
        disconnect(m_controller, &DmxController::sliderStatesChanged,
                   this, &DeviceRegisterValueHelper::sliderStatesChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
        connect(m_controller, &DmxController::sliderStatesChanged,
                this, &DeviceRegisterValueHelper::sliderStatesChanged);
    }

    emit controllerChanged(m_controller);
}

void DeviceRegisterValueHelper::setDeviceId(int deviceId)
{
    if (m_deviceId == deviceId)
        return;

    m_deviceId = deviceId;

    emit deviceIdChanged(m_deviceId);
}

void DeviceRegisterValueHelper::setRegisterIndex(int registerIndex)
{
    if (m_registerIndex == registerIndex)
        return;

    m_registerIndex = registerIndex;

    emit registerIndexChanged(m_registerIndex);
}

quint8 DeviceRegisterValueHelper::value() const
{
    if (!m_controller)
    {
//        qDebug() << "hilfe" << __LINE__;
        return 0;
    }
    if (m_deviceId == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return 0;
    }
    if (m_registerIndex == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return 0;
    }

    const auto &devices = m_controller->lightProject().devices;
    auto devicePtr = devices.findById(m_deviceId);
    if (!devicePtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return 0;
    }

    const auto index = devicePtr - &*std::cbegin(devices);

    const auto &sliderStates = m_controller->sliderStates();

    if (sliderStates.size() <= index)
        return 0;

    const auto &sliderState = sliderStates.at(index);

    if (sliderState.size() <= m_registerIndex)
        return 0;

    return sliderState.at(m_registerIndex);
}

void DeviceRegisterValueHelper::setValue(quint8 value)
{
    if (m_deviceId == -1)
    {
//        qDebug() << "hilfe" << __LINE__;
        return;
    }
    if (m_registerIndex == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }
    if (m_registerIndex == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto &devices = m_controller->lightProject().devices;
    auto devicePtr = devices.findById(m_deviceId);
    if (!devicePtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto index = devicePtr - &*std::cbegin(devices);

    auto &sliderStates = m_controller->sliderStates();

    if (sliderStates.size() <= index)
        sliderStates.resize(index + 1);

    auto &sliderState = sliderStates.at(index);

    if (sliderState.size() <= m_registerIndex)
        sliderState.resize(m_registerIndex + 1);

    {
        QMutexLocker locker{&m_controller->mutex()};
        sliderState[m_registerIndex] = value;
    }
    emit valueChanged(value);
}

void DeviceRegisterValueHelper::sliderStatesChanged()
{
    emit valueChanged(value());
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<DeviceRegisterValueHelper>("lightcontrol", 1, 0, "DeviceRegisterValueHelper");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)

