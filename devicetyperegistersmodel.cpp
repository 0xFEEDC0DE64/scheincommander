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

    if (m_controller)
    {
        disconnect(m_controller, &DmxController::deviceTypeRegisterInserted,
                   this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterInserted);
        disconnect(m_controller, &DmxController::deviceTypeRegisterRemoved,
                   this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterRemoved);
        disconnect(m_controller, &DmxController::deviceTypeRegisterTypeChanged,
                   this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterTypeChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
        connect(m_controller, &DmxController::deviceTypeRegisterInserted,
                this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterInserted);
        connect(m_controller, &DmxController::deviceTypeRegisterRemoved,
                this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterRemoved);
        connect(m_controller, &DmxController::deviceTypeRegisterTypeChanged,
                this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterTypeChanged);
    }

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
        return false;
    }

    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &deviceType = *deviceTypePtr;

    if (index.row() < 0 || index.row() >= deviceType.registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &deviceTypeRegister = deviceType.registers.at(index.row());

    switch (role)
    {
    case Qt::EditRole:
        deviceTypeRegister.type = value.value<DeviceTypeRegisterType>();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });

        disconnect(m_controller, &DmxController::deviceTypeRegisterTypeChanged,
                   this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterTypeChanged);
        emit m_controller->deviceTypeRegisterTypeChanged(deviceType, index.row(), deviceTypeRegister.type);
        connect(m_controller, &DmxController::deviceTypeRegisterTypeChanged,
                this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterTypeChanged);

        return true;

    default:
        qWarning() << "hilfe" << __LINE__;
        return false;
    }
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
        return false;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &deviceType = *deviceTypePtr;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &registers = deviceType.registers;

    beginInsertRows({}, row, row+count-1);
    auto iter = std::begin(registers) + row;
    for (auto i = 0; i < count; i++)
        iter = registers.insert(iter, DeviceTypeRegisterConfig{ .type = DeviceTypeRegisterType::Dummy }) + 1;
    endInsertRows();

    disconnect(m_controller, &DmxController::deviceTypeRegisterInserted,
               this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterInserted);
    emit m_controller->deviceTypeRegisterInserted(deviceType, row, row+count-1);
    connect(m_controller, &DmxController::deviceTypeRegisterInserted,
            this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterInserted);

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
        return false;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto deviceTypePtr = m_controller->lightProject().deviceTypes.findById(m_deviceTypeId);
    if (!deviceTypePtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &deviceType = *deviceTypePtr;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &registers = deviceType.registers;

    if (row >= registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > registers.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    auto begin = std::begin(registers) + row;
    auto end = begin + count;
    registers.erase(begin, end);
    endRemoveRows();

    disconnect(m_controller, &DmxController::deviceTypeRegisterRemoved,
               this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterRemoved);
    emit m_controller->deviceTypeRegisterRemoved(deviceType, row, row+count-1);
    connect(m_controller, &DmxController::deviceTypeRegisterRemoved,
            this, &DeviceTypeRegistersModel::otherDeviceTypeRegisterRemoved);

    return true;
}

void DeviceTypeRegistersModel::otherDeviceTypeRegisterInserted(const DeviceTypeConfig &deviceType, int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_deviceTypeId != deviceType.id)
        return;

    beginInsertRows({}, first, last);
    endInsertRows();
}

void DeviceTypeRegistersModel::otherDeviceTypeRegisterRemoved(const DeviceTypeConfig &deviceType, int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_deviceTypeId != deviceType.id)
        return;

    beginRemoveRows({}, first, last);
    endRemoveRows();
}

void DeviceTypeRegistersModel::otherDeviceTypeRegisterTypeChanged(const DeviceTypeConfig &deviceType, int row, DeviceTypeRegisterType type)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_deviceTypeId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_deviceTypeId != deviceType.id)
        return;

    const auto index = this->index(row);
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
}

namespace {
void registerDenShit()
{
    qmlRegisterType<DeviceTypeRegistersModel>("com.büro", 1, 0, "DeviceTypeRegistersModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registerDenShit)

