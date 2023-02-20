#ifndef PROJECTLOADER_H
#define PROJECTLOADER_H

#include <expected>
#include <vector>

#include <QJsonArray>
#include <QJsonObject>

#include "lightproject.h"

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

        }

        template <detail::LoadableFromArray T>
        std::expected<T, QString> load(const QJsonValue &json) {
            if (!json.isArray())
                return std::unexpected("Not a JSON array");
            return load<T>(json.toArray());
        }

        template <detail::LoadableFromObject T>
        std::expected<T, QString> load(const QJsonValue &json) {
            if (!json.isObject())
                return std::unexpected("Not a JSON object");
            return load<T>(json.toObject());
        }

        template<detail::Listific T>
        std::expected<T, QString> load(const QJsonArray &json) {
            T result;

            for (size_t i = 0; i < json.size(); i++) {
                auto &el = json[i];
                auto res = load<typename T::value_type>(el);
                if (res.has_value()) {
                    result.push_back(res.value());
                } else {
                    return std::unexpected(QString("%1: %2").arg(i).arg(res.error()));
                }
            }

            return result;
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

        template<std::same_as<QVector3D> T>
        std::expected<T, QString> load(const QJsonArray &json) {
            QVector3D vec;

            if (json.size() != 3) {
                return std::unexpected("Vector has wrong number of components");
            }

            for (size_t i = 0; i < json.size(); i++) {
                auto &el = json[i];
                if (auto val = load<float>(el); val) {
                    vec[i] = val.value();
                } else {
                    return std::unexpected(QString("%1: %2").arg(i).arg(val.error()));
                }
            }

            return vec;
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
    } // namespace

    std::expected<LightProject, QString> loadProject(const QJsonObject &json) {
        return load<LightProject>(json);
    }
} // namespace ProjectLoader

#endif // PROJECTLOADER_H
