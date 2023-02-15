#include "devicetypesmodel.h"

#include <QCoreApplication>
#include <QQmlEngine>

constexpr auto IdRole = Qt::UserRole;

DeviceTypesModel::DeviceTypesModel(QObject *parent) :
    QAbstractItemModel{parent}
{
}

void DeviceTypesModel::setController(DmxController *controller)
{
    if (m_controller != controller)
    {
        beginResetModel();
        m_controller = controller;
        endResetModel();
        emit controllerChanged(m_controller);
    }
}

QModelIndex DeviceTypesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    return createIndex(row, column, nullptr);
}

QModelIndex DeviceTypesModel::parent(const QModelIndex &child) const
{
    return {};
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

    return m_controller->lightProject().lightTypes.size();
}

int DeviceTypesModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return -1;
    }
    return 1;
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

    if (index.row() < 0 || index.row() >= m_controller->lightProject().lightTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &lightType = m_controller->lightProject().lightTypes.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole: return lightType.name;
    case IdRole:       return lightType.id;
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

    if (index.row() < 0 || index.row() >= m_controller->lightProject().lightTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &lightType = m_controller->lightProject().lightTypes.at(index.row());

    return {
        { Qt::DisplayRole, lightType.name },
        { IdRole,          lightType.id }
    };
}

QHash<int, QByteArray> DeviceTypesModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "name" },
        { IdRole,          "id" }
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

    auto &lightTypes = m_controller->lightProject().lightTypes;
    if (index.row() < 0 || index.row() >= lightTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &lightType = lightTypes.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (value.userType() != QMetaType::QString)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        lightType.name = value.toString();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
        return true;
    case IdRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        lightType.id = value.toInt();
        emit dataChanged(index, index, { IdRole });
        return true;
    }

    return false;
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

    auto &lightTypes = m_controller->lightProject().lightTypes;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row > lightTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginInsertRows({}, row, row+count-1);
    lightTypes.insert(std::begin(lightTypes) + row, count, LightTypeConfig{ .id=99, .name="<neu>" });
    endInsertRows();

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

    auto &lightTypes = m_controller->lightProject().lightTypes;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row >= lightTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > lightTypes.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    auto begin = std::begin(lightTypes) + row;
    auto end = begin + count;
    lightTypes.erase(begin, end);
    endRemoveRows();

    return true;
}

namespace {
void registerDenShit()
{
    qmlRegisterType<DeviceTypesModel>("com.büro", 1, 0, "DeviceTypesModel");
}
}
Q_COREAPP_STARTUP_FUNCTION(registerDenShit)
