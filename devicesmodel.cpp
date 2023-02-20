#include "devicesmodel.h"

#include <algorithm>

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>
#include <QMutexLocker>

enum {
    IdRole = Qt::UserRole,
    DeviceTypeIdRole,
    AddressRole,
    PositionRole
};

void DevicesModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    beginResetModel();

    if (m_controller)
    {
        disconnect(m_controller, &DmxController::deviceInserted,
                   this, &DevicesModel::otherDeviceInserted);
        disconnect(m_controller, &DmxController::deviceRemoved,
                   this, &DevicesModel::otherDeviceRemoved);
        disconnect(m_controller, &DmxController::deviceNameChanged,
                   this, &DevicesModel::otherDeviceNameChanged);
        disconnect(m_controller, &DmxController::deviceDeviceTypeIdChanged,
                   this, &DevicesModel::otherDeviceDeviceTypeIdChanged);
        disconnect(m_controller, &DmxController::deviceAddressChanged,
                   this, &DevicesModel::otherDeviceAddressChanged);
        disconnect(m_controller, &DmxController::devicePositionChanged,
                   this, &DevicesModel::otherDevicePositionChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
        connect(m_controller, &DmxController::deviceInserted,
                this, &DevicesModel::otherDeviceInserted);
        connect(m_controller, &DmxController::deviceRemoved,
                this, &DevicesModel::otherDeviceRemoved);
        connect(m_controller, &DmxController::deviceNameChanged,
                this, &DevicesModel::otherDeviceNameChanged);
        connect(m_controller, &DmxController::deviceDeviceTypeIdChanged,
                this, &DevicesModel::otherDeviceDeviceTypeIdChanged);
        connect(m_controller, &DmxController::deviceAddressChanged,
                this, &DevicesModel::otherDeviceAddressChanged);
        connect(m_controller, &DmxController::devicePositionChanged,
                this, &DevicesModel::otherDevicePositionChanged);
    }

    endResetModel();
    emit controllerChanged(m_controller);
}

int DevicesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return -1;
    }

    if (!m_controller)
        return 0;

    const auto &devices = m_controller->lightProject().devices;
    return devices.size();
}

QVariant DevicesModel::data(const QModelIndex &index, int role) const
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

    const auto &devices = m_controller->lightProject().devices;
    if (index.row() < 0 || index.row() >= devices.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &device = devices.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:    return device.name;
    case IdRole:          return device.id;
    case DeviceTypeIdRole: return device.deviceTypeId;
    case AddressRole:     return device.address;
    case PositionRole:    return device.position;
    }

    return {};
}

QMap<int, QVariant> DevicesModel::itemData(const QModelIndex &index) const
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

    const auto &devices = m_controller->lightProject().devices;
    if (index.row() < 0 || index.row() >= devices.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &device = devices.at(index.row());
    return {
        { Qt::DisplayRole, device.name },
        { IdRole,          device.id },
        { DeviceTypeIdRole, device.deviceTypeId },
        { AddressRole,     device.address },
        { PositionRole,    device.position }
    };
}

QHash<int, QByteArray> DevicesModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "name" },
        { IdRole,          "id" },
        { DeviceTypeIdRole, "deviceTypeId" },
        { AddressRole,     "address" },
        { PositionRole,    "position" }
    };
}

bool DevicesModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

    auto &devices = m_controller->lightProject().devices;
    if (index.row() < 0 || index.row() >= devices.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &device = devices.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (value.userType() != QMetaType::QString)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        device.name = value.toString();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });

        disconnect(m_controller, &DmxController::deviceNameChanged,
                   this, &DevicesModel::otherDeviceNameChanged);
        emit m_controller->deviceNameChanged(index.row(), device.name);
        connect(m_controller, &DmxController::deviceNameChanged,
                this, &DevicesModel::otherDeviceNameChanged);

        return true;
    case IdRole:
//        if (value.userType() != QMetaType::Int)
//        {
//            qWarning() << "hilfe" << __LINE__ << value.userType();
//            return false;
//        }
//        device.id = value.toInt();
//        emit dataChanged(index, index, { IdRole });
//        return true;
        qWarning() << "hilfe" << __LINE__;
        return false;
    case DeviceTypeIdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }

        {
            QMutexLocker locker{&m_controller->mutex()};
            device.deviceTypeId = value.toInt();
        }
        emit dataChanged(index, index, { DeviceTypeIdRole });

        disconnect(m_controller, &DmxController::deviceDeviceTypeIdChanged,
                   this, &DevicesModel::otherDeviceDeviceTypeIdChanged);
        emit m_controller->deviceDeviceTypeIdChanged(index.row(), device.deviceTypeId);
        connect(m_controller, &DmxController::deviceDeviceTypeIdChanged,
                this, &DevicesModel::otherDeviceDeviceTypeIdChanged);

        return true;
    case AddressRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }

        {
            QMutexLocker locker{&m_controller->mutex()};
            device.address = value.toInt();
        }
        emit dataChanged(index, index, { AddressRole });

        disconnect(m_controller, &DmxController::deviceAddressChanged,
                   this, &DevicesModel::otherDeviceAddressChanged);
        emit m_controller->deviceAddressChanged(index.row(), device.address);
        connect(m_controller, &DmxController::deviceAddressChanged,
                this, &DevicesModel::otherDeviceAddressChanged);

        return true;
    case PositionRole:
        if (value.userType() != QMetaType::QVector3D)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }

        {
            QMutexLocker locker{&m_controller->mutex()};
            device.position = value.value<QVector3D>();
        }
        emit dataChanged(index, index, { PositionRole });

        disconnect(m_controller, &DmxController::devicePositionChanged,
                   this, &DevicesModel::otherDevicePositionChanged);
        emit m_controller->devicePositionChanged(index.row(), device.position);
        connect(m_controller, &DmxController::devicePositionChanged,
                this, &DevicesModel::otherDevicePositionChanged);

        return true;
    default:
        qWarning() << "hilfe" << __LINE__;
        return false;
    }
}

bool DevicesModel::insertRows(int row, int count, const QModelIndex &parent)
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

    auto &devices = m_controller->lightProject().devices;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row > devices.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto max_iter = std::max_element(std::cbegin(devices), std::cend(devices), [](const auto &l, const auto &r){ return l.id < r.id; });
    auto id = max_iter != std::cend(devices) ? max_iter->id + 1 : 0;

    beginInsertRows({}, row, row+count-1);
    {
        QMutexLocker locker{&m_controller->mutex()};
        auto iter = std::begin(devices) + row;
        for (auto i = 0; i < count; i++)
            iter = devices.insert(iter, DeviceConfig{ .id=id++, .name="<neu>", .deviceTypeId=0, .address=0, .position={} }) + 1;
    }
    endInsertRows();

    disconnect(m_controller, &DmxController::deviceInserted,
               this, &DevicesModel::otherDeviceInserted);
    emit m_controller->deviceInserted(row, row+count-1);
    connect(m_controller, &DmxController::deviceInserted,
            this, &DevicesModel::otherDeviceInserted);

    return true;
}

bool DevicesModel::removeRows(int row, int count, const QModelIndex &parent)
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

    auto &devices = m_controller->lightProject().devices;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row >= devices.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > devices.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    {
        QMutexLocker locker{&m_controller->mutex()};
        auto begin = std::begin(devices) + row;
        auto end = begin + count;
        devices.erase(begin, end);
    }
    endRemoveRows();

    disconnect(m_controller, &DmxController::deviceRemoved,
               this, &DevicesModel::otherDeviceRemoved);
    emit m_controller->deviceRemoved(row, row+count-1);
    connect(m_controller, &DmxController::deviceRemoved,
            this, &DevicesModel::otherDeviceRemoved);

    return true;
}

void DevicesModel::otherDeviceInserted(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginInsertRows({}, first, last);
    endInsertRows();
}

void DevicesModel::otherDeviceRemoved(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginRemoveRows({}, first, last);
    endRemoveRows();
}

void DevicesModel::otherDeviceNameChanged(int row, const QString &name)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
}

void DevicesModel::otherDeviceDeviceTypeIdChanged(int row, int deviceTypeId)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { DeviceTypeIdRole });
}

void DevicesModel::otherDeviceAddressChanged(int row, int address)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { AddressRole });
}

void DevicesModel::otherDevicePositionChanged(int row, const QVector3D &position)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { PositionRole });
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<DevicesModel>("lightcontrol", 1, 0, "DevicesModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)
