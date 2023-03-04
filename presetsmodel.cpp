#include "presetsmodel.h"

#include <algorithm>

#include <QDebug>
#include <QMutexLocker>

enum {
    IdRole = Qt::UserRole,
    MsecsPerStepRole
};

void PresetsModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    beginResetModel();

    if (m_controller)
    {
        disconnect(m_controller, &DmxController::presetInserted,
                   this, &PresetsModel::otherPresetInserted);
        disconnect(m_controller, &DmxController::presetRemoved,
                   this, &PresetsModel::otherPresetRemoved);
        disconnect(m_controller, &DmxController::presetNameChanged,
                   this, &PresetsModel::otherPresetNameChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
        connect(m_controller, &DmxController::presetInserted,
                this, &PresetsModel::otherPresetInserted);
        connect(m_controller, &DmxController::presetRemoved,
                this, &PresetsModel::otherPresetRemoved);
        connect(m_controller, &DmxController::presetNameChanged,
                this, &PresetsModel::otherPresetNameChanged);
    }

    endResetModel();
    emit controllerChanged(m_controller);
}

int PresetsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return -1;
    }

    if (!m_controller)
        return 0;

    const auto &presets = m_controller->lightProject().presets;
    return presets.size();
}

QVariant PresetsModel::data(const QModelIndex &index, int role) const
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

    const auto &presets = m_controller->lightProject().presets;

    if (index.row() < 0 || index.row() >= presets.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &preset = presets.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:     return preset.name;
    case IdRole:           return preset.id;
    case MsecsPerStepRole: return preset.msecsPerStep;
    }

    return {};
}

QMap<int, QVariant> PresetsModel::itemData(const QModelIndex &index) const
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

    const auto &presets = m_controller->lightProject().presets;

    if (index.row() < 0 || index.row() >= presets.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &preset = presets.at(index.row());

    return {
        { Qt::DisplayRole,  preset.name },
        { IdRole,           preset.id },
        { MsecsPerStepRole, preset.msecsPerStep }
    };
}

QHash<int, QByteArray> PresetsModel::roleNames() const
{
    return {
        { Qt::DisplayRole,  "name" },
        { IdRole,           "id" },
        { MsecsPerStepRole, "msecsPerStep" }
    };
}

bool PresetsModel::setData(const QModelIndex &index, const QVariant &value, int role)
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

    auto &presets = m_controller->lightProject().presets;
    if (index.row() < 0 || index.row() >= presets.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &preset = presets.at(index.row());
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (value.userType() != QMetaType::QString)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        preset.name = value.toString();
        emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });

        disconnect(m_controller, &DmxController::presetNameChanged,
                   this, &PresetsModel::otherPresetNameChanged);
        emit m_controller->presetNameChanged(index.row(), preset.name);
        connect(m_controller, &DmxController::presetNameChanged,
                this, &PresetsModel::otherPresetNameChanged);

        return true;
    case IdRole:
//        if (value.userType() != QMetaType::Int)
//        {
//            qWarning() << "hilfe" << __LINE__ << value.userType();
//            return false;
//        }
//        preset.id = value.toInt();
//        emit dataChanged(index, index, { IdRole });
//        return true;
        qWarning() << "hilfe" << __LINE__;
        return false;
    case MsecsPerStepRole:
        if (value.userType() != QMetaType::Int)
        {
            qWarning() << "hilfe" << __LINE__ << value.userType();
            return false;
        }
        preset.msecsPerStep = value.toInt();
        emit dataChanged(index, index, { MsecsPerStepRole });
        return true;
    default:
        qWarning() << "hilfe" << __LINE__;
        return false;
    }
}

bool PresetsModel::insertRows(int row, int count, const QModelIndex &parent)
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

    auto &presets = m_controller->lightProject().presets;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row > presets.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto max_iter = std::max_element(std::cbegin(presets), std::cend(presets), [](const auto &l, const auto &r){ return l.id < r.id; });
    auto id = max_iter != std::cend(presets) ? max_iter->id + 1 : 0;

    beginInsertRows({}, row, row+count-1);
    {
        QMutexLocker locker{&m_controller->mutex()};

        {
            auto iter = std::begin(presets) + row;
            for (auto i = 0; i < count; i++)
                iter = presets.insert(iter, PresetConfig{ .id=id++, .name="<neu>", .msecsPerStep=1000 }) + 1;
        }

        if (auto &presetStates = m_controller->presetStates(); presetStates.size() > row)
        {
            presetStates.insert(std::begin(presetStates) + row, count, {});
//            emit m_controller->presetStatesChanged(presetStates);
        }
    }
    endInsertRows();

    disconnect(m_controller, &DmxController::presetInserted,
               this, &PresetsModel::otherPresetInserted);
    emit m_controller->presetInserted(row, row+count-1);
    connect(m_controller, &DmxController::presetInserted,
            this, &PresetsModel::otherPresetInserted);

    return true;
}

bool PresetsModel::removeRows(int row, int count, const QModelIndex &parent)
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

    auto &presets = m_controller->lightProject().presets;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row >= presets.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > presets.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    {
        QMutexLocker locker{&m_controller->mutex()};

        {
            auto begin = std::begin(presets) + row;
            auto end = begin + count;
            presets.erase(begin, end);
        }

        if (auto &presetStates = m_controller->presetStates(); presetStates.size() > row)
        {
            auto begin = std::begin(presetStates) + row;
            auto end = begin + std::min<size_t>(count, presetStates.size() - row + count);
            presetStates.erase(begin, end);
            //emit m_controller->presetStatesChanged(presetStates);
        }
    }
    endRemoveRows();

    disconnect(m_controller, &DmxController::presetRemoved,
               this, &PresetsModel::otherPresetRemoved);
    emit m_controller->presetRemoved(row, row+count-1);
    connect(m_controller, &DmxController::presetRemoved,
            this, &PresetsModel::otherPresetRemoved);

    return true;
}

void PresetsModel::otherPresetInserted(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginInsertRows({}, first, last);
    endInsertRows();
}

void PresetsModel::otherPresetRemoved(int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    beginRemoveRows({}, first, last);
    endRemoveRows();
}

void PresetsModel::otherPresetNameChanged(int row, const QString &name)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    const auto index = this->index(row);
    emit dataChanged(index, index, { Qt::DisplayRole, Qt::EditRole });
}
