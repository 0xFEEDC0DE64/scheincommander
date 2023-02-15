#pragma once

#include <QAbstractItemModel>

#include "dmxcontroller.h"

class DeviceTypesModel : public QAbstractItemModel
{
    Q_OBJECT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)

public:
    explicit DeviceTypesModel(QObject *parent = nullptr);

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

signals:
    void controllerChanged(DmxController *controller);

private:
    DmxController *m_controller{};
};
