#include "registergroupsmodel.h"

#include <algorithm>

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

    if (m_controller)
    {
        disconnect(m_controller, &DmxController::registerGroupInserted,
                   this, &RegisterGroupsModel::otherRegisterGroupInserted);
        disconnect(m_controller, &DmxController::registerGroupRemoved,
                   this, &RegisterGroupsModel::otherRegisterGroupRemoved);
        disconnect(m_controller, &DmxController::registerGroupNameChanged,
                   this, &RegisterGroupsModel::otherRegisterGroupNameChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
        connect(m_controller, &DmxController::registerGroupInserted,
                this, &RegisterGroupsModel::otherRegisterGroupInserted);
        connect(m_controller, &DmxController::registerGroupRemoved,
                this, &RegisterGroupsModel::otherRegisterGroupRemoved);
        connect(m_controller, &DmxController::registerGroupNameChanged,
                this, &RegisterGroupsModel::otherRegisterGroupNameChanged);
    }

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

        disconnect(m_controller, &DmxController::registerGroupNameChanged,
                   this, &RegisterGroupsModel::otherRegisterGroupNameChanged);
        emit m_controller->registerGroupNameChanged(index.row(), registerGroup.name);
        connect(m_controller, &DmxController::registerGroupNameChanged,
                this, &RegisterGroupsModel::otherRegisterGroupNameChanged);

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

    auto &registerGroups = m_controller->lightProject().registerGroups;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row > registerGroups.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto max_iter = std::max_element(std::cbegin(registerGroups), std::cend(registerGroups), [](const auto &l, const auto &r){ return l.id < r.id; });
    auto id = max_iter != std::cend(registerGroups) ? max_iter->id + 1 : 0;

    beginInsertRows({}, row, row+count-1);
    auto iter = std::begin(registerGroups) + row;
    for (auto i = 0; i < count; i++)
        iter = registerGroups.insert(iter, RegisterGroupConfig{ .id=id++, .name="<neu>" }) + 1;
    endInsertRows();

    disconnect(m_controller, &DmxController::registerGroupInserted,
               this, &RegisterGroupsModel::otherRegisterGroupInserted);
    emit m_controller->registerGroupInserted(row, row+count-1);
    connect(m_controller, &DmxController::registerGroupInserted,
            this, &RegisterGroupsModel::otherRegisterGroupInserted);

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

    auto &registerGroups = m_controller->lightProject().registerGroups;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row >= registerGroups.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > registerGroups.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    auto begin = std::begin(registerGroups) + row;
    auto end = begin + count;
    registerGroups.erase(begin, end);
    endRemoveRows();

    disconnect(m_controller, &DmxController::registerGroupRemoved,
               this, &RegisterGroupsModel::otherRegisterGroupRemoved);
    emit m_controller->registerGroupRemoved(row, row+count-1);
    connect(m_controller, &DmxController::registerGroupRemoved,
            this, &RegisterGroupsModel::otherRegisterGroupRemoved);

    return true;
}

void RegisterGroupsModel::otherRegisterGroupInserted(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginInsertRows({}, first, last);
    endInsertRows();
}

void RegisterGroupsModel::otherRegisterGroupRemoved(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginRemoveRows({}, first, last);
    endRemoveRows();
}

void RegisterGroupsModel::otherRegisterGroupNameChanged(int row, const QString &name)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
}

namespace {
void registrierDenShit()
{
    qmlRegisterType<RegisterGroupsModel>("lightcontrol", 1, 0, "RegisterGroupsModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)
