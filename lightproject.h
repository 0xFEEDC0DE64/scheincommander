#pragma once

#include <vector>
#include <algorithm>

#include <QString>
#include <QVector3D>
#include <QMetaEnum>
#include <QMetaType>

namespace scheincommander {
Q_NAMESPACE
enum class DeviceTypeRegisterType
{
    Dummy,
    Dimmer,
    Red,
    Green,
    Blue,
    White,
    Shutter,
    Pan,
    PanFine,
    Tilt,
    TiltFine,
    Color,
    Cyan,
    Magenta,
    Yellow,
    Gobo,
    Rotation,
    Prism,
    Frost,
    Focus,
    Speed,
    Iris,
    Zoom,
    Extra1,
    Extra2
};
Q_ENUM_NS(DeviceTypeRegisterType)
} // namespace scheincommander

Q_DECLARE_METATYPE(scheincommander::DeviceTypeRegisterType)

using DeviceTypeRegisterType = scheincommander::DeviceTypeRegisterType;

struct DeviceTypeRegisterConfig
{
    DeviceTypeRegisterType type;
};

struct DeviceTypeConfig
{
    int id;
    QString name;
    QString iconName;
    std::vector<DeviceTypeRegisterConfig> registers;
};

class DeviceTypesContainer : public std::vector<DeviceTypeConfig>
{
    using base_t = std::vector<DeviceTypeConfig>;

public:
    using base_t::base_t;

    DeviceTypeConfig *findById(int id)
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const DeviceTypeConfig &deviceType){ return deviceType.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }

    const DeviceTypeConfig *findById(int id) const
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const DeviceTypeConfig &deviceType){ return deviceType.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }
};

struct DeviceConfig
{
    int id;
    QString name;
    int deviceTypeId;
    int address;
    QVector3D position;
};

class DevicesContainer : public std::vector<DeviceConfig>
{
    using base_t = std::vector<DeviceConfig>;

public:
    using base_t::base_t;

    DeviceConfig *findById(int id)
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const DeviceConfig &device){ return device.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }

    const DeviceConfig *findById(int id) const
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const DeviceConfig &device){ return device.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }
};

using sliders_state_t = std::vector<std::vector<quint8>>;

struct PresetStepConfig
{
    sliders_state_t sliders;
};

struct PresetConfig
{
    int id;
    QString name;
    int msecsPerStep;
    std::vector<PresetStepConfig> steps;
};

class PresetsContainer : public std::vector<PresetConfig>
{
    using base_t = std::vector<PresetConfig>;

public:
    using base_t::base_t;

    PresetConfig *findById(int id)
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const PresetConfig &preset){ return preset.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }

    const PresetConfig *findById(int id) const
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const PresetConfig &preset){ return preset.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }
};

struct LightProject
{
    DeviceTypesContainer deviceTypes;
    DevicesContainer devices;
    PresetsContainer presets;
};
