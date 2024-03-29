#pragma once

#include <QAbstractListModel>
#include <qqml.h>

#include "dmxcontroller.h"

class DeviceTypesModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)

public:
    using QAbstractListModel::QAbstractListModel;

    enum Roles {
        IdRole = Qt::UserRole,
        IconNameRole,
        IconUrlRole
    };
    Q_ENUM(Roles)

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    Q_INVOKABLE QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

signals:
    void controllerChanged(DmxController *controller);

private slots:
    void otherDeviceTypeInserted(int first, int last);
    void otherDeviceTypeRemoved(int first, int last);
    void otherDeviceTypeNameChanged(int row, const QString &name);
    void otherDeviceTypeIconNameChanged(int row, const QString &name);

private:
    DmxController *m_controller{};
};
