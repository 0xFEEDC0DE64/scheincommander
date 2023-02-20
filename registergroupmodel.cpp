#include "registergroupmodel.h"

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QMutexLocker>

void RegisterGroupModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    if (m_controller)
    {
//        disconnect(m_controller, &DmxController::registerGroupInserted,
//                   this, &RegisterGroupsModel::otherRegisterGroupInserted);
    }

    m_controller = controller;

    if (m_controller)
    {
//        connect(m_controller, &DmxController::registerGroupInserted,
//                this, &RegisterGroupsModel::otherRegisterGroupInserted);
    }

    emit controllerChanged(m_controller);
}

void RegisterGroupModel::setRegisterGroupId(int registerGroupId)
{
    if (m_registerGroupId == registerGroupId)
        return;

    m_registerGroupId = registerGroupId;

    emit registerGroupIdChanged(m_registerGroupId);
}

void RegisterGroupModel::copyFromFaders()
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    if (m_registerGroupId == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    auto registerGroupPtr = m_controller->lightProject().registerGroups.findById(m_registerGroupId);
    if (!registerGroupPtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    auto &registerGroup = *registerGroupPtr;

    {
        QMutexLocker locker{&m_controller->mutex()};
        registerGroup.sliders = m_controller->sliderStates();
    }
}

void RegisterGroupModel::copyToFaders()
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    if (m_registerGroupId == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto registerGroupPtr = m_controller->lightProject().registerGroups.findById(m_registerGroupId);
    if (!registerGroupPtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto &registerGroup = *registerGroupPtr;

    m_controller->setSliderStates(registerGroup.sliders);
}

void RegisterGroupModel::setAllFadersLow()
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto &devices = m_controller->lightProject().devices;

    sliders_state_t sliderStates;
    sliderStates.reserve(devices.size());

    for (const auto &device : devices)
    {
        std::vector<quint8> sliderState;

        if (device.deviceTypeId != -1)
        {
            const auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(device.deviceTypeId);
            if (!deviceTypePtr)
                qWarning() << "hilfe" << __LINE__;
            else
            {
                const auto &deviceType = *deviceTypePtr;
                sliderState.reserve(deviceType.registers.size());

                for (const auto &register_ : deviceType.registers)
                    sliderState.push_back(0);
            }
        }

        sliderStates.push_back(std::move(sliderState));
    }

    m_controller->setSliderStates(std::move(sliderStates));
}

void RegisterGroupModel::setAllFadersMax()
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto &devices = m_controller->lightProject().devices;

    sliders_state_t sliderStates;
    sliderStates.reserve(devices.size());

    for (const auto &device : devices)
    {
        std::vector<quint8> sliderState;

        if (device.deviceTypeId != -1)
        {
            const auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(device.deviceTypeId);
            if (!deviceTypePtr)
                qWarning() << "hilfe" << __LINE__;
            else
            {
                const auto &deviceType = *deviceTypePtr;
                sliderState.reserve(deviceType.registers.size());

                for (const auto &register_ : deviceType.registers)
                    sliderState.push_back(255);
            }
        }

        sliderStates.push_back(std::move(sliderState));
    }

    m_controller->setSliderStates(std::move(sliderStates));
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<RegisterGroupModel>("lightcontrol", 1, 0, "RegisterGroupModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)

