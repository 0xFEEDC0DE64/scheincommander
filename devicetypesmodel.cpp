#include "devicetypesmodel.h"

#include <algorithm>

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>

enum {
    IdRole = Qt::UserRole,
    IconNameRole
};

void DeviceTypesModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    beginResetModel();

    if (m_controller)
    {
        disconnect(m_controller, &DmxController::deviceTypeInserted,
                   this, &DeviceTypesModel::otherDeviceTypeInserted);
        disconnect(m_controller, &DmxController::deviceTypeRemoved,
                   this, &DeviceTypesModel::otherDeviceTypeRemoved);
        disconnect(m_controller, &DmxController::deviceTypeNameChanged,
                   this, &DeviceTypesModel::otherDeviceTypeNameChanged);
        disconnect(m_controller, &DmxController::deviceTypeIconNameChanged,
                   this, &DeviceTypesModel::otherDeviceTypeIconNameChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
        connect(m_controller, &DmxController::deviceTypeInserted,
                this, &DeviceTypesModel::otherDeviceTypeInserted);
        connect(m_controller, &DmxController::deviceTypeRemoved,
                this, &DeviceTypesModel::otherDeviceTypeRemoved);
        connect(m_controller, &DmxController::deviceTypeNameChanged,
                this, &DeviceTypesModel::otherDeviceTypeNameChanged);
        connect(m_controller, &DmxController::deviceTypeIconNameChanged,
                this, &DeviceTypesModel::otherDeviceTypeIconNameChanged);
    }

    endResetModel();
    emit controllerChanged(m_controller);
}

int DeviceTypesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return -1;
    }

    if (!m_controller)
        return 0;

    const auto &deviceTypes = m_controller->lightProject().deviceTypes;
    return deviceTypes.size();
}

QVariant DeviceTypesModel::data(const QModelIndex &index, int role) const
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

    const auto &deviceTypes = m_controller->lightProject().deviceTypes;

    if (index.row() < 0 || index.row() >= deviceTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &deviceType = deviceTypes.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole: return deviceType.name;
    case IdRole:       return deviceType.id;
    case IconNameRole: return deviceType.iconName;
    }

    return {};
}

QMap<int, QVariant> DeviceTypesModel::itemData(const QModelIndex &index) const
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

    const auto &deviceTypes = m_controller->lightProject().deviceTypes;

    if (index.row() < 0 || index.row() >= deviceTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &deviceType = deviceTypes.at(index.row());

    return {
        { Qt::DisplayRole, deviceType.name },
        { IdRole,          deviceType.id },
        { IconNameRole,    deviceType.iconName }
    };
}

QHash<int, QByteArray> DeviceTypesModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "name" },
        { IdRole,          "id" },
        { IconNameRole,    "iconName" }
    };
}

bool DeviceTypesModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

    auto &deviceTypes = m_controller->lightProject().deviceTypes;
    if (index.row() < 0 || index.row() >= deviceTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &deviceType = deviceTypes.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (value.userType() != QMetaType::QString)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        deviceType.name = value.toString();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });

        disconnect(m_controller, &DmxController::deviceTypeNameChanged,
                   this, &DeviceTypesModel::otherDeviceTypeNameChanged);
        emit m_controller->deviceTypeNameChanged(index.row(), deviceType.name);
        connect(m_controller, &DmxController::deviceTypeNameChanged,
                this, &DeviceTypesModel::otherDeviceTypeNameChanged);

        return true;
    case IdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        deviceType.id = value.toInt();
        emit dataChanged(index, index, { IdRole });
        return true;
    case IconNameRole:
        if (value.userType() != QMetaType::QString)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        deviceType.iconName = value.toString();
        emit dataChanged(index, index, { IconNameRole });

        disconnect(m_controller, &DmxController::deviceTypeIconNameChanged,
                   this, &DeviceTypesModel::otherDeviceTypeIconNameChanged);
        emit m_controller->deviceTypeIconNameChanged(index.row(), deviceType.iconName);
        connect(m_controller, &DmxController::deviceTypeIconNameChanged,
                this, &DeviceTypesModel::otherDeviceTypeIconNameChanged);

        return true;
    default:
        qWarning() << "hilfe" << __LINE__;
        return false;
    }
}

bool DeviceTypesModel::insertRows(int row, int count, const QModelIndex &parent)
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

    auto &deviceTypes = m_controller->lightProject().deviceTypes;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row > deviceTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto max_iter = std::max_element(std::cbegin(deviceTypes), std::cend(deviceTypes), [](const auto &l, const auto &r){ return l.id < r.id; });
    auto id = max_iter != std::cend(deviceTypes) ? max_iter->id + 1 : 0;

    beginInsertRows({}, row, row+count-1);
    auto iter = std::begin(deviceTypes) + row;
    for (auto i = 0; i < count; i++)
        iter = deviceTypes.insert(iter, DeviceTypeConfig{ .id=id++, .name="<neu>" }) + 1;
    endInsertRows();

    disconnect(m_controller, &DmxController::deviceTypeInserted,
               this, &DeviceTypesModel::otherDeviceTypeInserted);
    emit m_controller->deviceTypeInserted(row, row+count-1);
    connect(m_controller, &DmxController::deviceTypeInserted,
            this, &DeviceTypesModel::otherDeviceTypeInserted);

    return true;
}

bool DeviceTypesModel::removeRows(int row, int count, const QModelIndex &parent)
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

    auto &deviceTypes = m_controller->lightProject().deviceTypes;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row >= deviceTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > deviceTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    auto begin = std::begin(deviceTypes) + row;
    auto end = begin + count;
    deviceTypes.erase(begin, end);
    endRemoveRows();

    disconnect(m_controller, &DmxController::deviceTypeRemoved,
               this, &DeviceTypesModel::otherDeviceTypeRemoved);
    emit m_controller->deviceTypeRemoved(row, row+count-1);
    connect(m_controller, &DmxController::deviceTypeRemoved,
            this, &DeviceTypesModel::otherDeviceTypeRemoved);

    return true;
}

void DeviceTypesModel::otherDeviceTypeInserted(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginInsertRows({}, first, last);
    endInsertRows();
}

void DeviceTypesModel::otherDeviceTypeRemoved(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginRemoveRows({}, first, last);
    endRemoveRows();
}

void DeviceTypesModel::otherDeviceTypeNameChanged(int row, const QString &name)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
}

void DeviceTypesModel::otherDeviceTypeIconNameChanged(int row, const QString &name)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { IconNameRole });
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<DeviceTypesModel>("lightcontrol", 1, 0, "DeviceTypesModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)
