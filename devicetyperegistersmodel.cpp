#include "devicetyperegistersmodel.h"

#include <utility>

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>

void DeviceTypeRegistersModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    beginResetModel();
    m_controller = controller;
    endResetModel();
    emit controllerChanged(m_controller);
}

void DeviceTypeRegistersModel::setDeviceTypeId(int deviceTypeId)
{
    if (m_deviceTypeId == deviceTypeId)
        return;

    beginResetModel();
    m_deviceTypeId = deviceTypeId;
    endResetModel();
    emit deviceTypeIdChanged(m_deviceTypeId);
}

int DeviceTypeRegistersModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return -1;
    }

    if (!m_controller)
        return 0;

    if (m_deviceTypeId == -1)
        return 0;

    auto lightType = m_controller->lightProject().lightTypes.findById(m_deviceTypeId);
    if (!lightType)
    {
        qWarning() << "hilfe" << __LINE__;
        return 0;
    }

    return lightType->registers.size();
}

QVariant DeviceTypeRegistersModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    auto lightType = m_controller->lightProject().lightTypes.findById(m_deviceTypeId);
    if (!lightType)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.row() < 0 || index.row() >= lightType->registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &lightTypeRegister = lightType->registers.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return QMetaEnum::fromType<LightTypeRegisterType>().valueToKey(std::to_underlying(lightTypeRegister.type));
    }
    case Qt::EditRole:
        return QVariant::fromValue(lightTypeRegister.type);
    }

    return {};
}

QMap<int, QVariant> DeviceTypeRegistersModel::itemData(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    auto lightType = m_controller->lightProject().lightTypes.findById(m_deviceTypeId);
    if (!lightType)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.row() < 0 || index.row() >= lightType->registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &lightTypeRegister = lightType->registers.at(index.row());

    return {
        { Qt::DisplayRole, QMetaEnum::fromType<LightTypeRegisterType>().valueToKey(std::to_underlying(lightTypeRegister.type)) },
        { Qt::EditRole,    QVariant::fromValue(lightTypeRegister.type) }
    };
}

QHash<int, QByteArray> DeviceTypeRegistersModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "registerTypeName" },
        { Qt::EditRole,    "registerType" },
    };
}

namespace {
void registerDenShit()
{
    qmlRegisterType<DeviceTypeRegistersModel>("com.büro", 1, 0, "DeviceTypeRegistersModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registerDenShit)

