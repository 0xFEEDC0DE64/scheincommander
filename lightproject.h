#pragma once

#include <vector>
#include <algorithm>

#include <QString>
#include <QVector3D>
#include <QMetaEnum>
#include <QMetaType>

namespace hilfe {
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
} // namespace hilfe

Q_DECLARE_METATYPE(hilfe::DeviceTypeRegisterType)

using DeviceTypeRegisterType = hilfe::DeviceTypeRegisterType;

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

struct RegisterGroupConfig
{
    int id;
    QString name;
    sliders_state_t sliders;
};

class RegisterGroupsContainer : public std::vector<RegisterGroupConfig>
{
    using base_t = std::vector<RegisterGroupConfig>;

public:
    using base_t::base_t;

    RegisterGroupConfig *findById(int id)
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const RegisterGroupConfig &registerGroup){ return registerGroup.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }

    const RegisterGroupConfig *findById(int id) const
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const RegisterGroupConfig &registerGroup){ return registerGroup.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }
};

struct LightProject
{
    DeviceTypesContainer deviceTypes;
    DevicesContainer devices;
    RegisterGroupsContainer registerGroups;
};
