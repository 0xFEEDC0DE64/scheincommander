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

namespace {
void registrierDenShit()
{
    qmlRegisterType<RegisterGroupsModel>("lightcontrol", 1, 0, "RegisterGroupsModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registrierDenShit)
