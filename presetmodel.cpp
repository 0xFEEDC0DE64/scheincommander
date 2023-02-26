#include "presetmodel.h"

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QMutexLocker>

void PresetModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    if (m_controller)
    {
//        disconnect(m_controller, &DmxController::presetInserted,
//                   this, &PresetsModel::otherPresetInserted);
    }

    m_controller = controller;

    if (m_controller)
    {
//        connect(m_controller, &DmxController::presetInserted,
//                this, &PresetsModel::otherPresetInserted);
    }

    emit controllerChanged(m_controller);
}

void PresetModel::setPresetId(int presetId)
{
    if (m_presetId == presetId)
        return;

    m_presetId = presetId;

    emit presetIdChanged(m_presetId);
}

void PresetModel::copyFromFaders()
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    auto presetPtr = m_controller->lightProject().presets.findById(m_presetId);
    if (!presetPtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    auto &preset = *presetPtr;

    {
        QMutexLocker locker{&m_controller->mutex()};
        preset.steps = { { .sliders=m_controller->sliderStates() } };
    }
}

void PresetModel::copyToFaders()
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto presetPtr = m_controller->lightProject().presets.findById(m_presetId);
    if (!presetPtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto &preset = *presetPtr;

    if (preset.steps.empty())
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    m_controller->setSliderStates(preset.steps.front().sliders);
}

void PresetModel::setAllFadersLow()
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

void PresetModel::setAllFadersMax()
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

void PresetModel::setPattern(int n, int k, DeviceTypeRegisterType registerType, quint8 value)
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto &devices = m_controller->lightProject().devices;

    sliders_state_t sliderStates;
    sliderStates.reserve(devices.size());

    int i{};
    for (const auto &device : devices)
    {
        const bool test = i++ % n == k;

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
                    sliderState.push_back(test && register_.type == registerType ? value : 0);
            }
        }

        sliderStates.push_back(std::move(sliderState));
    }

    m_controller->setSliderStates(std::move(sliderStates));
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<PresetModel>("scheincommander", 1, 0, "PresetModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)

