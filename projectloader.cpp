#include <concepts>
#include <list>
#include <type_traits>
#include <vector>

#include <QJsonArray>
#include <QJsonObject>

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
                   std::same_as<T, PresetsContainer>;

template<typename T>
concept OtherLoadableFromArray = std::same_as<T, QVector3D>;

template<typename T>
concept LoadableFromArray = Listific<T> || OtherLoadableFromArray<T>;

template<typename T>
concept LoadableFromObject = std::same_as<T, LightProject> ||
                             std::same_as<T, DeviceConfig> ||
                             std::same_as<T, DeviceTypeConfig> ||
                             std::same_as<T, DeviceTypeRegisterConfig> ||
                             std::same_as<T, PresetConfig> ||
                             std::same_as<T, PresetStepConfig>;

template<typename T>
concept JsonNumber = (std::integral<T> && !std::same_as<T, bool> && !std::is_enum_v<T>) ||
                     std::floating_point<T>;

// TODO: Can we match on Q_ENUMs only?
template<typename T>
concept LoadableQEnum = std::is_enum_v<T>;

template<typename T, typename U>
concept SameAsQualified = std::same_as<std::remove_cv_t<T>, U>;

} // namespace detail


// forward declarations for loading stuff from QJsonValue
template <detail::LoadableFromObject T>
std::expected<T, QString> load(const QJsonValue &json);
template <detail::LoadableFromArray T>
std::expected<T, QString> load(const QJsonValue &json);
template<detail::JsonNumber T>
std::expected<T, QString> load(const QJsonValue &json);


// iterateMembers stuff
template<detail::SameAsQualified<LightProject> T, typename CB>
void iterateMembers(T &ref, CB &&cb)
{
    cb("deviceTypes", ref.deviceTypes);
    cb("devices", ref.devices);
    cb("presets", ref.presets);
}

template<detail::SameAsQualified<DeviceTypeConfig> T, typename CB>
void iterateMembers(T &ref, CB &&cb)
{
    cb("id", ref.id);
    cb("name", ref.name);
    cb("iconName", ref.iconName);
    cb("registers", ref.registers);
}

template<detail::SameAsQualified<DeviceConfig> T, typename CB>
void iterateMembers(T &ref, CB &&cb)
{
    cb("id", ref.id);
    cb("name", ref.name);
    cb("deviceTypeId", ref.deviceTypeId);
    cb("address", ref.address);
    cb("position", ref.position);
}

template<detail::SameAsQualified<PresetConfig> T, typename CB>
void iterateMembers(T &ref, CB &&cb)
{
    cb("id", ref.id);
    cb("name", ref.name);
    cb("msecsPerStep", ref.msecsPerStep);
    cb("steps", ref.steps);
}

template<detail::SameAsQualified<DeviceTypeRegisterConfig> T, typename CB>
void iterateMembers(T &ref, CB &&cb)
{
    cb("type", ref.type);
}

template<detail::SameAsQualified<PresetStepConfig> T, typename CB>
void iterateMembers(T &ref, CB &&cb)
{
    cb("sliders", ref.sliders);
}

template<detail::Listific T>
std::expected<T, QString> load(const QJsonArray &json) {
    T result;

    for (size_t i = 0; i < json.size(); i++) {
        const auto &el = json[i];
        auto res = load<typename T::value_type>(el);
        if (res.has_value()) {
            result.push_back(res.value());
        } else {
            return tl::make_unexpected(QString("%1: %2").arg(i).arg(res.error()));
        }
    }

    return result;
}

template<std::same_as<QVector3D> T>
std::expected<T, QString> load(const QJsonArray &json) {
    QVector3D vec;

    if (json.size() != 3) {
        return tl::make_unexpected("Vector has wrong number of components");
    }

    for (size_t i = 0; i < json.size(); i++) {
        const auto &el = json[i];
        if (auto val = load<float>(el); val) {
            vec[i] = val.value();
        } else {
            return tl::make_unexpected(QString("%1: %2").arg(i).arg(val.error()));
        }
    }

    return vec;
}

template <detail::LoadableFromArray T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (!json.isArray())
        return tl::make_unexpected("Not a JSON array");
    return load<T>(json.toArray());
}

template<detail::JsonNumber T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (json.isDouble()) {
        return (T)json.toDouble();
    } else {
        return tl::make_unexpected(QString("Not a number"));
    }
}

template<std::same_as<QString> T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (json.isString()) {
        return json.toString();
    } else {
        return tl::make_unexpected(QString("Not a string"));
    }
}

template<detail::LoadableQEnum T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (!json.isString()) {
        return tl::make_unexpected("Not a JSON string");
    }

    QMetaEnum me = QMetaEnum::fromType<T>();
    if (!me.isValid()) {
        return tl::make_unexpected("Invalid QMetaEnum");
    }

    bool ok = false;
    auto value = me.keyToValue(json.toString().toStdString().c_str(), &ok);
    if (ok) {
        return (T)value;
    } else {
        return tl::make_unexpected(QString("No value found for enum key %1").arg(json.toString()));
    }
}

template<typename T>
std::expected<T, QString> loadIfExists(const QJsonObject &json, const QString &key) {
    if (json.contains(key)) {
        return load<T>(json[key]);
    } else {
        return tl::make_unexpected(QString("does not exist"));
    }
}

template<detail::LoadableFromObject T>
std::expected<T, QString> load(const QJsonObject &json) {
    T t;
    std::optional<tl::unexpected<QString>> error;

    iterateMembers(t, [&json, &error] (const char *name, auto &field) {
        if (error)
            return;

        if (auto result = loadIfExists<typename std::remove_cvref_t<decltype(field)>>(json, name)) {
            field = std::move(result.value());
        } else {
            error = tl::make_unexpected(QString("%1: %2").arg(name).arg(result.error()));
        }
    });

    if (error)
        return *error;

    return t;
}

template <detail::LoadableFromObject T>
std::expected<T, QString> load(const QJsonValue &json) {
    if (!json.isObject())
        return tl::make_unexpected("Not a JSON object");
    return load<T>(json.toObject());
}

} // namespace

std::expected<LightProject, QString> loadProject(const QJsonDocument &json) {
    if (!json.isObject()) {
        return tl::make_unexpected("JsonDocument is not an object");
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
        return tl::make_unexpected("Invalid QMetaEnum");
    }

    if (auto key = me.valueToKey((int)val); key) {
        return QJsonValue(key);
    } else {
        return tl::make_unexpected(QString("No key found for enum value %1").arg((int)val));
    }
}

template<detail::LoadableFromObject T>
std::expected<QJsonObject, QString> save(const T &val) {
    QJsonObject json;
    std::optional<tl::unexpected<QString>> error;

    iterateMembers(val, [&json, &error] (const char *name, auto &field) {
        if (error)
            return;

        if (auto result = save<typename std::remove_cvref_t<decltype(field)>>(field)) {
            json[name] = result.value();
        } else {
            error = tl::make_unexpected(QString("%1: %2").arg(name).arg(result.error()));
        }
    });

    if (error)
        return *error;

    return json;
}

template <detail::Listific T>
std::expected<QJsonArray, QString> save(const T &val) {
    QJsonArray arr;

    size_t i = 0;
    for (auto it = std::cbegin(val); it != std::cend(val); it++, i++) {
        auto json = save<typename T::value_type>(*it);
        if (json) {
            arr.push_back(json.value());
        } else {
            return tl::make_unexpected(QString("%1: %2").arg(i).arg(json.error()));
        }
    }

    return arr;
}

} // namespace

std::expected<QJsonDocument, QString> saveProject(const LightProject &val) {
    if (auto project = save<LightProject>(val); project) {
        return QJsonDocument(project.value());
    } else {
        return tl::make_unexpected(QString("Failed to save project: %1").arg(project.error()));
    }
}

} // namespace ProjectLoader
