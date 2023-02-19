#include "registergroupsmodel.h"

#include <QDebug>
#include <QCoreApplication>
#include <QQmlEngine>

enum {
    IdRole = Qt::UserRole,
};

void RegisterGroupsModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    beginResetModel();
    m_controller = controller;
    endResetModel();
    emit controllerChanged(m_controller);
}

int RegisterGroupsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return -1;
    }

    if (!m_controller)
        return 0;

    const auto &registerGroups = m_controller->lightProject().registerGroups;
    return registerGroups.size();
}

QVariant RegisterGroupsModel::data(const QModelIndex &index, int role) const
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

    const auto &registerGroups = m_controller->lightProject().registerGroups;

    if (index.row() < 0 || index.row() >= registerGroups.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &registerGroup = registerGroups.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole: return registerGroup.name;
    case IdRole:       return registerGroup.id;
    }

    return {};
}

QMap<int, QVariant> RegisterGroupsModel::itemData(const QModelIndex &index) const
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

    const auto &registerGroups = m_controller->lightProject().registerGroups;

    if (index.row() < 0 || index.row() >= registerGroups.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &registerGroup = registerGroups.at(index.row());

    return {
        { Qt::DisplayRole, registerGroup.name },
        { IdRole,          registerGroup.id }
    };
}

QHash<int, QByteArray> RegisterGroupsModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "name" },
        { IdRole,          "id" }
    };
}

bool RegisterGroupsModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

    auto &registerGroups = m_controller->lightProject().registerGroups;
    if (index.row() < 0 || index.row() >= registerGroups.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &registerGroup = registerGroups.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (value.userType() != QMetaType::QString)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        registerGroup.name = value.toString();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
        return true;
    case IdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        registerGroup.id = value.toInt();
        emit dataChanged(index, index, { IdRole });
        return true;
    default:
        qWarning() << "hilfe" << __LINE__;
        return false;
    }
}

bool RegisterGroupsModel::insertRows(int row, int count, const QModelIndex &parent)
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
    for (auto i = 0; i < count; i++)
        iter = devices.insert(iter, LightConfig{ .id=id++, .name="<neu>", .deviceTypeId=0, .address=0, .position={} }) + 1;
    endInsertRows();

    return true;
}

bool RegisterGroupsModel::removeRows(int row, int count, const QModelIndex &parent)
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
    qmlRegisterType<RegisterGroupsModel>("lightcontrol", 1, 0, "RegisterGroupsModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)
