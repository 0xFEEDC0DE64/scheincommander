#pragma once

#include <QAbstractListModel>
#include <qqml.h>

#include "dmxcontroller.h"

class PresetStepsModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(int presetId READ presetId WRITE setPresetId NOTIFY presetIdChanged)

public:
    using QAbstractListModel::QAbstractListModel;

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    int presetId() const { return m_presetId; }
    void setPresetId(int presetId);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void controllerChanged(DmxController *controller);
    void presetIdChanged(int presetId);

private:
    DmxController *m_controller{};
    int m_presetId{-1};
};
