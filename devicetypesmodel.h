#pragma once

#include <QAbstractItemModel>

#include "dmxcontroller.h"

class DeviceTypesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)

public:
    using QAbstractListModel::QAbstractListModel;

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

signals:
    void controllerChanged(DmxController *controller);

private slots:
    void otherDeviceTypeInserted(int first, int last);
    void otherDeviceTypeRemoved(int first, int last);
    void otherDeviceTypeNameChanged(int row, const QString &name);

private:
    DmxController *m_controller{};
};
