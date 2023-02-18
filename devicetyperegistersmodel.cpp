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

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return 0;
    }

    const auto &deviceType = *deviceTypePtr;

    return deviceType.registers.size();
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

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &deviceType = *deviceTypePtr;

    if (index.row() < 0 || index.row() >= deviceType.registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &deviceTypeRegister = deviceType.registers.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    {
        return QMetaEnum::fromType<DeviceTypeRegisterType>().valueToKey(std::to_underlying(deviceTypeRegister.type));
    }
    case Qt::EditRole:
        return QVariant::fromValue(deviceTypeRegister.type);
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

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    auto &deviceType = *deviceTypePtr;

    if (index.row() < 0 || index.row() >= deviceType.registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &deviceTypeRegister = deviceType.registers.at(index.row());

    return {
        { Qt::DisplayRole, QMetaEnum::fromType<DeviceTypeRegisterType>().valueToKey(std::to_underlying(deviceTypeRegister.type)) },
        { Qt::EditRole,    QVariant::fromValue(deviceTypeRegister.type) }
    };
}

QHash<int, QByteArray> DeviceTypeRegistersModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "registerTypeName" },
        { Qt::EditRole,    "registerType" },
    };
}

bool DeviceTypeRegistersModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto &deviceType = *deviceTypePtr;

    if (index.row() < 0 || index.row() >= deviceType.registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto &deviceTypeRegister = deviceType.registers.at(index.row());

    deviceTypeRegister.type = value.value<DeviceTypeRegisterType>();
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
    return true;
}

bool DeviceTypeRegistersModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto &deviceType = *deviceTypePtr;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto &registers = deviceType.registers;

    beginInsertRows({}, row, row+count-1);
    auto iter = std::begin(registers) + row;
    for (; count > 0; count--)
        iter = registers.insert(iter, DeviceTypeRegisterConfig{ .type = DeviceTypeRegisterType::Dummy }) + 1;
    endInsertRows();

    return true;
}

bool DeviceTypeRegistersModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto &deviceType = *deviceTypePtr;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    auto &registers = deviceType.registers;

    if (row >= registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    if (row + count > registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return true;
    }

    beginRemoveRows({}, row, row+count-1);
    auto begin = std::begin(registers) + row;
    auto end = begin + count;
    registers.erase(begin, end);
    endRemoveRows();

    return true;
}

namespace {
void registerDenShit()
{
    qmlRegisterType<DeviceTypeRegistersModel>("com.büro", 1, 0, "DeviceTypeRegistersModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registerDenShit)

