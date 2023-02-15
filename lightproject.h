#pragma once

#include <vector>
#include <algorithm>

#include <QString>
#include <QVector3D>
#include <QMetaEnum>
#include <QMetaType>

namespace hilfe {
Q_NAMESPACE
enum class LightTypeRegisterType
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
Q_ENUM_NS(LightTypeRegisterType)
} // namespace hilfe

Q_DECLARE_METATYPE(hilfe::LightTypeRegisterType)

using LightTypeRegisterType = hilfe::LightTypeRegisterType;

struct LightTypeRegisterConfig
{
    LightTypeRegisterType type;
};

struct LightTypeConfig
{
    int id;
    QString name;
    std::vector<LightTypeRegisterConfig> registers;
};

class LightTypesContainer : public std::vector<LightTypeConfig>
{
    using base_t = std::vector<LightTypeConfig>;

public:
    using base_t::base_t;

    LightTypeConfig *findById(int id)
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const LightTypeConfig &lightType){ return lightType.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }

    const LightTypeConfig *findById(int id) const
    {
        auto iter = std::find_if(std::begin(*this), std::end(*this),
                                 [&id](const LightTypeConfig &lightType){ return lightType.id == id; });
        return iter != std::end(*this) ? &*iter : nullptr;
    }
};

struct LightConfig
{
    int id;
    QString name;
    int lightTypeId;
    int address;
    QVector3D position;
};

struct LightProject
{
    LightTypesContainer lightTypes;
    std::vector<LightConfig> lights;
};
