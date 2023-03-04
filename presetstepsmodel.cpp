#include "presetstepsmodel.h"

#include <QDebug>

void PresetStepsModel::setController(DmxController *controller)
{
    if (m_controller == controller)
        return;

    beginResetModel();

    if (m_controller)
    {
//        disconnect(m_controller, &DmxController::presetRegisterInserted,
//                   this, &PresetStepsModel::otherPresetRegisterInserted);
//        disconnect(m_controller, &DmxController::presetRegisterRemoved,
//                   this, &PresetStepsModel::otherPresetRegisterRemoved);
//        disconnect(m_controller, &DmxController::presetRegisterTypeChanged,
//                   this, &PresetStepsModel::otherPresetRegisterTypeChanged);
    }

    m_controller = controller;

    if (m_controller)
    {
//        connect(m_controller, &DmxController::presetRegisterInserted,
//                this, &PresetStepsModel::otherPresetRegisterInserted);
//        connect(m_controller, &DmxController::presetRegisterRemoved,
//                this, &PresetStepsModel::otherPresetRegisterRemoved);
//        connect(m_controller, &DmxController::presetRegisterTypeChanged,
//                this, &PresetStepsModel::otherPresetRegisterTypeChanged);
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
