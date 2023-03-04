#include "patternmaker.h"

void PatternMaker::setController(DmxController *controller)
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

void PatternMaker::setAllFadersLow()
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

void PatternMaker::setAllFadersMax()
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

void PatternMaker::setPattern(int n, int k, DeviceTypeRegisterType registerType, quint8 value)
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
