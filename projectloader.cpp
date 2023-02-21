#include "projectloader.h"

namespace ProjectLoader {

namespace {

namespace detail {

template<typename Test, template<typename...> class Ref>
struct is_specialization_t : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization_t<Ref<Args...>, Ref>: std::true_type {};

// broken
//template<typename ...T>
//constexpr bool is_specialization_v = is_specialization_t<T...>::value;

template<typename T>
concept Listific = is_specialization_t<T, std::list>::value ||
                   is_specialization_t<T, std::vector>::value ||
                   std::same_as<T, DeviceTypesContainer> ||
                   std::same_as<T, DevicesContainer> ||
                   std::same_as<T, RegisterGroupsContainer>;

template<typename T>
concept OtherLoadableFromArray = std::same_as<T, QVector3D>;

template<typename T>
concept LoadableFromArray = Listific<T> || OtherLoadableFromArray<T>;

template<typename T>
concept LoadableFromObject = std::same_as<T, LightProject> ||
                             std::same_as<T, DeviceConfig> ||
                             std::same_as<T, DeviceTypeConfig> ||
                             std::same_as<T, DeviceTypeRegisterConfig> ||
                             std::same_as<T, RegisterGroupConfig>;

template<typename T>
concept JsonNumber = (std::integral<T> && !std::same_as<T, bool> && !std::is_enum_v<T>) ||
                     std::floating_point<T>;

// TODO: Can we match on Q_ENUMs only?
template<typename T>
concept LoadableQEnum = std::is_enum_v<T>;

} // namespace detail

// forward declarations for loading stuff from QJsonValue
template <detail::LoadableFromObject T>
std::expected<T, QString> load(const QJsonValue &json);
template <detail::LoadableFromArray T>
std::expected<T, QString> load(const QJsonValue &json);
template<detail::JsonNumber T>
std::expected<T, QString> load(const QJsonValue &json);

template<detail::Listific T>
std::expected<T, QString> load(const QJsonArray &json) {
    T result;

    for (size_t i = 0; i < json.size(); i++) {
        const auto &el = json[i];
        auto res = load<typename T::value_type>(el);
        if (res.has_value()) {
            result.push_back(res.value());
        } else {
            return std::unexpected(QString("%1: %2").arg(i).arg(res.error()));
        }
    }

    return result;
}

template<std::same_as<QVector3D> T>
std::expected<T, QString> load(const QJsonArray &json) {
    QVector3D vec;

    if (json.size() != 3) {
        return std::unexpected("Vector has wrong number of components");
    }

    for (size_t i = 0; i < json.size(); i++) {
        const auto &el = json[i];
        if (auto val = load<float>(el); val) {
            vec[i] = val.value();
        } else {
            return std::unexpected(QString("%1: %2").arg(i).arg(val.error()));
        }
    }

    return vec;
}

template <detail::LoadableFromArray T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (!json.isArray())
        return std::unexpected("Not a JSON array");
    return load<T>(json.toArray());
}

template<detail::JsonNumber T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (json.isDouble()) {
        return (T)json.toDouble();
    } else {
        return std::unexpected(QString("Not a number"));
    }
}

template<std::same_as<QString> T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (json.isString()) {
        return json.toString();
    } else {
        return std::unexpected(QString("Not a string"));
    }
}

template<detail::LoadableQEnum T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (!json.isString()) {
        return std::unexpected("Not a JSON string");
    }

    QMetaEnum me = QMetaEnum::fromType<T>();
    if (!me.isValid()) {
        return std::unexpected("Invalid QMetaEnum");
    }

    bool ok = false;
    auto value = me.keyToValue(json.toString().toStdString().c_str(), &ok);
    if (ok) {
        return (T)value;
    } else {
        return std::unexpected(QString("No value found for enum key %1").arg(json.toString()));
    }
}

template<typename T>
std::expected<T, QString> loadIfExists(const QJsonObject &json, const QString &key) {
    if (json.contains(key)) {
        return load<T>(json[key]);
    } else {
        return std::unexpected(QString("does not exist"));
    }
}

template<std::same_as<LightProject> T>
std::expected<T, QString> load(const QJsonObject &json) {
    LightProject lp;

    if (auto deviceTypes = loadIfExists<decltype(lp.deviceTypes)>(json, "deviceTypes"); deviceTypes) {
        lp.deviceTypes = std::move(deviceTypes.value());
    } else {
        return std::unexpected(QString("deviceTypes: %1").arg(deviceTypes.error()));
    }

    if (auto devices = loadIfExists<decltype(lp.devices)>(json, "devices"); devices) {
        lp.devices = std::move(devices.value());
    } else {
        return std::unexpected(QString("devices: %1").arg(devices.error()));
    }

    if (auto registerGroups = loadIfExists<decltype(lp.registerGroups)>(json, "registerGroups"); registerGroups) {
        lp.registerGroups = std::move(registerGroups.value());
    } else {
        return std::unexpected(QString("registerGroups: %1").arg(registerGroups.error()));
    }

    return lp;
}

template<std::same_as<DeviceTypeConfig> T>
std::expected<T, QString> load(const QJsonObject &json) {
    DeviceTypeConfig dtc;

    if (auto val = loadIfExists<decltype(dtc.id)>(json, "id"); val) {
        dtc.id = val.value();
    } else {
        return std::unexpected(QString("id: %1").arg(val.error()));
    }

    if (auto val = loadIfExists<decltype(dtc.name)>(json, "name"); val) {
        dtc.name = val.value();
    } else {
        return std::unexpected(QString("name: %1").arg(val.error()));
    }

    if (auto val = loadIfExists<decltype(dtc.iconName)>(json, "iconName"); val) {
        dtc.iconName = val.value();
    } else {
        return std::unexpected(QString("iconName: %1").arg(val.error()));
    }

    if (auto val = loadIfExists<decltype(dtc.registers)>(json, "registers"); val) {
        dtc.registers = std::move(val.value());
    } else {
        return std::unexpected(QString("registers: %1").arg(val.error()));
    }

    return dtc;
}

template<std::same_as<DeviceTypeRegisterConfig> T>
std::expected<T, QString> load(const QJsonObject &json) {
    DeviceTypeRegisterConfig dtrc;
    if (auto val = loadIfExists<decltype(dtrc.type)>(json, "type"); val) {
        dtrc.type = val.value();
    } else {
        return std::unexpected(QString("type: %1").arg(val.error()));
    }

    return dtrc;
}

template<std::same_as<DeviceConfig> T>
std::expected<T, QString> load(const QJsonObject &json) {
    DeviceConfig dc;
    if (auto val = loadIfExists<decltype(dc.id)>(json, "id"); val) {
        dc.id = val.value();
    } else {
        return std::unexpected(QString("id: %1").arg(val.error()));
    }
    if (auto val = loadIfExists<decltype(dc.name)>(json, "name"); val) {
        dc.name = val.value();
    } else {
        return std::unexpected(QString("name: %1").arg(val.error()));
    }
    if (auto val = loadIfExists<decltype(dc.deviceTypeId)>(json, "deviceTypeId"); val) {
        dc.deviceTypeId = val.value();
    } else {
        return std::unexpected(QString("deviceTypeId: %1").arg(val.error()));
    }
    if (auto val = loadIfExists<decltype(dc.address)>(json, "address"); val) {
        dc.address = val.value();
    } else {
        return std::unexpected(QString("address: %1").arg(val.error()));
    }
    if (auto val = loadIfExists<decltype(dc.position)>(json, "position"); val) {
        dc.position = val.value();
    } else {
        return std::unexpected(QString("position: %1").arg(val.error()));
    }

    return dc;
}

template<std::same_as<RegisterGroupConfig> T>
std::expected<T, QString> load(const QJsonObject &json) {
    RegisterGroupConfig rgc;
    if (auto val = loadIfExists<decltype(rgc.id)>(json, "id"); val) {
        rgc.id = val.value();
    } else {
        return std::unexpected(QString("id: %1").arg(val.error()));
    }
    if (auto val = loadIfExists<decltype(rgc.name)>(json, "name"); val) {
        rgc.name = val.value();
    } else {
        return std::unexpected(QString("name: %1").arg(val.error()));
    }
    if (auto val = loadIfExists<decltype(rgc.sliders)>(json, "sliders"); val) {
        rgc.sliders = val.value();
    } else {
        return std::unexpected(QString("sliders: %1").arg(val.error()));
    }

    return rgc;
}

template <detail::LoadableFromObject T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (!json.isObject())
        return std::unexpected("Not a JSON object");
    return load<T>(json.toObject());
}

} // namespace

std::expected<LightProject, QString> loadProject(const QJsonDocument &json) {
    if (!json.isObject()) {
        return std::unexpected("JsonDocument is not an object");
    }

    return load<LightProject>(json.object());
}

namespace {
template <detail::Listific T>
std::expected<QJsonArray, QString> save(const T &val);

template<detail::JsonNumber T>
std::expected<QJsonValue, QString> save(const T &val) {
    return QJsonValue((double)val);
}

template<std::same_as<QString> T>
std::expected<QJsonValue, QString> save(const T &val) {
    return QJsonValue(val);
}

template<std::same_as<QVector3D> T>
std::expected<QJsonArray, QString> save(const T &val) {
    return QJsonArray{val.x(), val.y(), val.z()};
}

template<detail::LoadableQEnum T>
std::expected<QJsonValue, QString> save(const T &val) {
    QMetaEnum me = QMetaEnum::fromType<T>();
    if (!me.isValid()) {
        return std::unexpected("Invalid QMetaEnum");
    }

    if (auto key = me.valueToKey((int)val); key) {
        return QJsonValue(key);
    } else {
        return std::unexpected(QString("No key found for enum value %1").arg((int)val));
    }
}

template<std::same_as<DeviceTypeConfig> T>
std::expected<QJsonObject, QString> save(const T &val) {
    QJsonObject json;

    if (auto id = save<decltype(val.id)>(val.id); id) {
        json["id"] = id.value();
    } else {
        return std::unexpected(QString("id: %1").arg(id.error()));
    }

    if (auto name = save<decltype(val.name)>(val.name); name) {
        json["name"] = name.value();
    } else {
        return std::unexpected(QString("name: %1").arg(name.error()));
    }

    if (auto iconName = save<decltype(val.iconName)>(val.iconName); iconName) {
        json["iconName"] = iconName.value();
    } else {
        return std::unexpected(QString("iconName: %1").arg(iconName.error()));
    }

    if (auto registers = save<decltype(val.registers)>(val.registers); registers) {
        json["registers"] = registers.value();
    } else {
        return std::unexpected(QString("registers: %1").arg(registers.error()));
    }

    return json;
}

template<std::same_as<DeviceTypeRegisterConfig> T>
std::expected<QJsonObject, QString> save(const T &val) {
    QJsonObject json;
    if (auto type = save<decltype(val.type)>(val.type); type) {
        json["type"] = type.value();
    } else {
        return std::unexpected(QString("type: %1").arg(type.error()));
    }

    return json;
}

template<std::same_as<DeviceConfig> T>
std::expected<QJsonObject, QString> save(const T &val) {
    QJsonObject json;
    if (auto id = save<decltype(val.id)>(val.id); id) {
        json["id"] = id.value();
    } else {
        return std::unexpected(QString("id: %1").arg(id.error()));
    }
    if (auto name = save<decltype(val.name)>(val.name); name) {
        json["name"] = name.value();
    } else {
        return std::unexpected(QString("name: %1").arg(name.error()));
    }
    if (auto deviceTypeId = save<decltype(val.deviceTypeId)>(val.deviceTypeId); deviceTypeId) {
        json["deviceTypeId"] = deviceTypeId.value();
    } else {
        return std::unexpected(QString("deviceTypeId: %1").arg(deviceTypeId.error()));
    }
    if (auto address = save<decltype(val.address)>(val.address); address) {
        json["address"] = address.value();
    } else {
        return std::unexpected(QString("address: %1").arg(address.error()));
    }
    if (auto position = save<decltype(val.position)>(val.position); position) {
        json["position"] = position.value();
    } else {
        return std::unexpected(QString("position: %1").arg(position.error()));
    }

    return json;
}

template<std::same_as<RegisterGroupConfig> T>
std::expected<QJsonObject, QString> save(const T &val) {
    QJsonObject json;
    if (auto id = save<decltype(val.id)>(val.id); id) {
        json["id"] = id.value();
    } else {
        return std::unexpected(QString("id: %1").arg(id.error()));
    }
    if (auto name = save<decltype(val.name)>(val.name); name) {
        json["name"] = name.value();
    } else {
        return std::unexpected(QString("name: %1").arg(name.error()));
    }
    if (auto sliders = save<decltype(val.sliders)>(val.sliders); sliders) {
        json["sliders"] = sliders.value();
    } else {
        return std::unexpected(QString("sliders: %1").arg(sliders.error()));
    }

    return json;
}

template <detail::Listific T>
std::expected<QJsonArray, QString> save(const T &val) {
    QJsonArray arr;

    for (size_t i = 0; i < val.size(); i++) {
        const auto &el = val[i];
        auto json = save<typename T::value_type>(el);
        if (json) {
            arr.push_back(json.value());
        } else {
            return std::unexpected(QString("%1: %2").arg(i).arg(json.error()));
        }
    }

    return arr;
}

template<std::same_as<LightProject> T>
std::expected<QJsonObject, QString> save(const T &val) {
    QJsonObject json;
    if (auto deviceTypes = save<decltype(val.deviceTypes)>(val.deviceTypes); deviceTypes) {
        json["deviceTypes"] = deviceTypes.value();
    } else {
        return std::unexpected(QString("deviceTypes: %1").arg(deviceTypes.error()));
    }

    if (auto devices = save<decltype(val.devices)>(val.devices); devices) {
        json["devices"] = devices.value();
    } else {
        return std::unexpected(QString("devices: %1").arg(devices.error()));
    }

    if (auto registerGroups = save<decltype(val.registerGroups)>(val.registerGroups); registerGroups) {
        json["registerGroups"] = registerGroups.value();
    } else {
        return std::unexpected(QString("registerGroups: %1").arg(registerGroups.error()));
    }

    return json;
}

} // namespace

std::expected<QJsonDocument, QString> saveProject(const LightProject &val) {
    if (auto project = save<LightProject>(val); project) {
        return QJsonDocument(project.value());
    } else {
        return std::unexpected(QString("Failed to save project: %1").arg(project.error()));
    }
}

} // namespace ProjectLoader
