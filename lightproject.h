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
    Strobo,
    Shutter
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

struct LightConfig
{
    int id;
    QString name;
    int deviceTypeId;
    int address;
    QVector3D position;
};

struct LightProject
{
    DeviceTypesContainer deviceTypes;
    std::vector<LightConfig> devices;
};
