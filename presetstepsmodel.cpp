#include "presetstepsmodel.h"

#include <QDebug>

void PresetStepsModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    beginResetModel();

    if (m_controller)
    {
        disconnect(m_controller, &DmxController::presetStepInserted,
                   this, &PresetStepsModel::otherPresetStepInserted);
        disconnect(m_controller, &DmxController::presetStepRemoved,
                   this, &PresetStepsModel::otherPresetStepRemoved);
//        disconnect(m_controller, &DmxController::presetStepTypeChanged,
//                   this, &PresetStepsModel::otherPresetStepTypeChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
        connect(m_controller, &DmxController::presetStepInserted,
                this, &PresetStepsModel::otherPresetStepInserted);
        connect(m_controller, &DmxController::presetStepRemoved,
                this, &PresetStepsModel::otherPresetStepRemoved);
//        connect(m_controller, &DmxController::presetStepTypeChanged,
//                this, &PresetStepsModel::otherPresetStepTypeChanged);
    }

    endResetModel();

    emit controllerChanged(m_controller);
}

void PresetStepsModel::setPresetId(int presetId)
{
    if (m_presetId == presetId)
        return;

    beginResetModel();
    m_presetId = presetId;
    endResetModel();
    emit presetIdChanged(m_presetId);
}

void PresetStepsModel::copyFromFaders(int stepIndex)
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    auto presetPtr = m_controller->lightProject().presets.findById(m_presetId);
    if (!presetPtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    auto &preset = *presetPtr;

    {
        QMutexLocker locker{&m_controller->mutex()};
        // TODO respect stepIndex
        preset.steps = { { .sliders=m_controller->sliderStates() } };
    }
}

void PresetStepsModel::copyToFaders(int stepIndex)
{
    if (!m_controller)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId == -1)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto presetPtr = m_controller->lightProject().presets.findById(m_presetId);
    if (!presetPtr)
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    const auto &preset = *presetPtr;

    // TODO respect stepIndex
    if (preset.steps.empty())
    {
        qDebug() << "hilfe" << __LINE__;
        return;
    }

    m_controller->setSliderStates(preset.steps.front().sliders);
}

int PresetStepsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        qWarning() << "hilfe" << __LINE__;
        return -1;
    }

    if (!m_controller)
        return 0;

    if (m_presetId == -1)
        return 0;

    const auto &presets = m_controller->lightProject().presets;
    auto presetPtr = presets.findById(m_presetId);
    if (!presetPtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return 0;
    }

    const auto &preset = *presetPtr;

    return preset.steps.size();
}

QVariant PresetStepsModel::data(const QModelIndex &index, int role) const
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

    if (m_presetId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &presets = m_controller->lightProject().presets;
    auto presetPtr = presets.findById(m_presetId);
    if (!presetPtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &preset = *presetPtr;

    const auto &steps = preset.steps;
    if (index.row() < 0 || index.row() >= steps.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &step = steps.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return tr("Step %0").arg(index.row() + 1);
    }

    return {};
}

QMap<int, QVariant> PresetStepsModel::itemData(const QModelIndex &index) const
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

    if (m_presetId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &presets = m_controller->lightProject().presets;
    auto presetPtr = presets.findById(m_presetId);
    if (!presetPtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &preset = *presetPtr;

    const auto &steps = preset.steps;
    if (index.row() < 0 || index.row() >= steps.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    if (index.column() != 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return {};
    }

    const auto &step = steps.at(index.row());

    return {
        { Qt::DisplayRole, tr("Step %0").arg(index.row() + 1) }
    };
}

QHash<int, QByteArray> PresetStepsModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "name" },
    };
}

bool PresetStepsModel::insertRows(int row, int count, const QModelIndex &parent)
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

    if (m_presetId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &presets = m_controller->lightProject().presets;
    auto presetPtr = presets.findById(m_presetId);
    if (!presetPtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &preset = *presetPtr;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &steps = preset.steps;

    beginInsertRows({}, row, row+count-1);
    {
        QMutexLocker locker{&m_controller->mutex()};
        auto iter = std::begin(steps) + row;
        for (auto i = 0; i < count; i++)
            iter = steps.insert(iter, PresetStepConfig{}) + 1;
    }
    endInsertRows();

    disconnect(m_controller, &DmxController::presetStepInserted,
               this, &PresetStepsModel::otherPresetStepInserted);
    emit m_controller->presetStepInserted(preset, row, row+count-1);
    connect(m_controller, &DmxController::presetStepInserted,
            this, &PresetStepsModel::otherPresetStepInserted);

    return true;
}

bool PresetStepsModel::removeRows(int row, int count, const QModelIndex &parent)
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

    if (m_presetId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &presets = m_controller->lightProject().presets;
    auto presetPtr = presets.findById(m_presetId);
    if (!presetPtr)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &preset = *presetPtr;

    if (row < 0)
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    auto &steps = preset.steps;

    if (row >= steps.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    if (row + count > steps.size())
    {
        qWarning() << "hilfe" << __LINE__;
        return false;
    }

    beginRemoveRows({}, row, row+count-1);
    {
        QMutexLocker locker{&m_controller->mutex()};
        auto begin = std::begin(steps) + row;
        auto end = begin + count;
        steps.erase(begin, end);
    }
    endRemoveRows();

    disconnect(m_controller, &DmxController::presetStepRemoved,
               this, &PresetStepsModel::otherPresetStepRemoved);
    emit m_controller->presetStepRemoved(preset, row, row+count-1);
    connect(m_controller, &DmxController::presetStepRemoved,
            this, &PresetStepsModel::otherPresetStepRemoved);

    return true;
}

void PresetStepsModel::otherPresetStepInserted(const PresetConfig &preset, int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId != preset.id)
        return;

    beginInsertRows({}, first, last);
    endInsertRows();
}

void PresetStepsModel::otherPresetStepRemoved(const PresetConfig &preset, int first, int last)
{
    if (!m_controller)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId == -1)
    {
        qWarning() << "hilfe" << __LINE__;
        return;
    }

    if (m_presetId != preset.id)
        return;

    beginRemoveRows({}, first, last);
    endRemoveRows();
}
