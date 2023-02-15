#include "devicesmodel.h"

#include <algorithm>

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>

constexpr auto IdRole = Qt::UserRole;
constexpr auto LightTypeIdRole = Qt::UserRole + 1;
constexpr auto AddressRole = Qt::UserRole + 2;
constexpr auto PositionRole = Qt::UserRole + 3;

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

    return m_controller->lightProject().lights.size();
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

    const auto &lights = m_controller->lightProject().lights;
    if (index.row() < 0 || index.row() >= lights.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &light = lights.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:    return light.name;
    case IdRole:          return light.id;
    case LightTypeIdRole: return light.lightTypeId;
    case AddressRole:     return light.address;
    case PositionRole:    return light.position;
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

    const auto &lights = m_controller->lightProject().lights;
    if (index.row() < 0 || index.row() >= lights.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &light = lights.at(index.row());

    return {
        { Qt::DisplayRole, light.name },
        { IdRole,          light.id },
        { LightTypeIdRole, light.lightTypeId },
        { AddressRole,     light.address },
        { PositionRole,    light.position }
    };
}

QHash<int, QByteArray> DevicesModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "name" },
        { IdRole,          "id" },
        { LightTypeIdRole, "lightTypeId" },
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

    auto &lights = m_controller->lightProject().lights;
    if (index.row() < 0 || index.row() >= lights.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &light = lights.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (value.userType() != QMetaType::QString)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        light.name = value.toString();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
        return true;
    case IdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        light.id = value.toInt();
        emit dataChanged(index, index, { IdRole });
        return true;
    case LightTypeIdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        light.lightTypeId = value.toInt();
        emit dataChanged(index, index, { LightTypeIdRole });
        return true;
    case AddressRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        light.address = value.toInt();
        emit dataChanged(index, index, { AddressRole });
        return true;
    case PositionRole:
        if (value.userType() != QMetaType::QVector3D)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        light.position = value.value<QVector3D>();
        emit dataChanged(index, index, { PositionRole });
        return true;
    }

    return false;
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

    auto &lights = m_controller->lightProject().lights;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row > lights.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto max_iter = std::max_element(std::cbegin(lights), std::cend(lights), [](const auto &l, const auto &r){ return l.id < r.id; });
    auto id = max_iter != std::cend(lights) ? max_iter->id + 1 : 0;

    beginInsertRows({}, row, row+count-1);
    auto iter = std::begin(lights) + row;
    for (; count > 0; count--)
        iter = lights.insert(iter, LightConfig{ .id=id++, .name="<neu>", .lightTypeId=0, .address=0, .position={} }) + 1;
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

    auto &lights = m_controller->lightProject().lights;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row >= lights.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > lights.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    auto begin = std::begin(lights) + row;
    auto end = begin + count;
    lights.erase(begin, end);
    endRemoveRows();

    return true;
}

namespace {
void registerDenShit()
{
    qmlRegisterType<DevicesModel>("com.büro", 1, 0, "DevicesModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registerDenShit)
