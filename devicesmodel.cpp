#include "devicesmodel.h"

#include <algorithm>

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>

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
    m_controller = controller;
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
        return true;
    case IdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        device.id = value.toInt();
        emit dataChanged(index, index, { IdRole });
        return true;
    case DeviceTypeIdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        device.deviceTypeId = value.toInt();
        emit dataChanged(index, index, { DeviceTypeIdRole });
        return true;
    case AddressRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        device.address = value.toInt();
        emit dataChanged(index, index, { AddressRole });
        return true;
    case PositionRole:
        if (value.userType() != QMetaType::QVector3D)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        device.position = value.value<QVector3D>();
        emit dataChanged(index, index, { PositionRole });
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
    auto iter = std::begin(devices) + row;
    for (; count > 0; count--)
        iter = devices.insert(iter, LightConfig{ .id=id++, .name="<neu>", .deviceTypeId=0, .address=0, .position={} }) + 1;
    endInsertRows();

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
    auto begin = std::begin(devices) + row;
    auto end = begin + count;
    devices.erase(begin, end);
    endRemoveRows();

    return true;
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<DevicesModel>("lightcontrol", 1, 0, "DevicesModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)
