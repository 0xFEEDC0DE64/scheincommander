import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import lightcontrol 1.0

Pane {
    property alias deviceTypeId: model.deviceTypeId

    Material.elevation: 6

    DeviceTypeRegistersModel {
        id: model
        controller: __controller
    }

    RowLayout {
        anchors.fill: parent

        Pane {
            Layout.preferredWidth: 300
            Layout.fillHeight: true

            Material.elevation: 6

            EditableListView {
                id: listView
                anchors.fill: parent

                textRole: 'registerTypeName'

                model: model

                onAddClicked: (index) => { const newIndex = index < 0 ? 0 : index + 1; if (model.insertRow(newIndex)) currentIndex = newIndex; else console.warn('failed'); }
                onRemoveClicked: (index) => model.removeRow(index)
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            enabled: listView.currentIndex >= 0

            GridLayout {
                Layout.fillWidth: true

                columns: 2

                Label {
                    text: qsTr('Type:')
                }
                ComboBox {
                    id: comboBox
                    model: DeviceTypeRegisterTypesModel {
                    }
                    textRole: "text"
                    valueRole: "value"

                    currentIndex: listView.currentData ? comboBox.indexOfValue(listView.currentData.registerType) : -1
                    onActivated: if (listView.currentData) listView.currentData.registerType = currentValue; else console.warn('discarded');
                }
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}
