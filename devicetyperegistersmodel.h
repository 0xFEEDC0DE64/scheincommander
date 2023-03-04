#pragma once

#include <QAbstractListModel>
#include <qqml.h>

#include "dmxcontroller.h"

class DeviceTypeRegistersModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(DmxController* controller READ controller WRITE setController NOTIFY controllerChanged)
    Q_PROPERTY(int deviceTypeId READ deviceTypeId WRITE setDeviceTypeId NOTIFY deviceTypeIdChanged)

public:
    using QAbstractListModel::QAbstractListModel;

    DmxController *controller() { return m_controller; }
    const DmxController *controller() const { return m_controller; }
    void setController(DmxController *controller);

    int deviceTypeId() const { return m_deviceTypeId; }
    void setDeviceTypeId(int deviceTypeId);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QMap<int, QVariant> itemData(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRows(int row, int count, const QModelIndex &parent) override;

signals:
    void controllerChanged(DmxController *controller);
    void deviceTypeIdChanged(int deviceTypeId);

private slots:
    void otherDeviceTypeRegisterInserted(const DeviceTypeConfig &deviceType, int first, int last);
    void otherDeviceTypeRegisterRemoved(const DeviceTypeConfig &deviceType, int first, int last);
    void otherDeviceTypeRegisterTypeChanged(const DeviceTypeConfig &deviceType, int row, DeviceTypeRegisterType type);

private:
    DmxController *m_controller{};
    int m_deviceTypeId{-1};
};
