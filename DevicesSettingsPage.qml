import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import scheincommander

ColumnLayout {
    Label {
        text: qsTr("Devices Settings")
    }

    RowLayout {
        //Layout.fillWidth: true
        Layout.fillHeight: true

        EditableListView {
            id: listView

            iconSourceRole: "iconUrl"

            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            model: DevicesModel {
                id: model
                controller: __controller
            }

            onAddClicked: (index) => { const newIndex = index < 0 ? 0 : index + 1; if (model.insertRow(newIndex)) currentIndex = newIndex; else console.warn('failed'); }
            onRemoveClicked: (index) => {
                const dialog = dialogComponent.createObject(Overlay.overlay);
                dialog.index = index;
                dialog.open();
            }

            onCurrentDataChanged: if (currentData) positionField.value = currentData.position

            Component {
                id: dialogComponent

                Dialog {
                    property int index

                    anchors.centerIn: parent
                    standardButtons: DialogButtonBox.Yes | DialogButtonBox.Cancel
                    modal: true
                    title: qsTr('Confirmation')

                    onAccepted: model.removeRow(index)

                    Label {
                        id: textContainer

                        anchors.fill: parent

                        horizontalAlignment: Qt.AlignLeft
                        verticalAlignment: Qt.AlignTop

                        text: qsTr('Are you sure you want to remove row %0').arg(index)
                    }
                }
            }
        }

        ColumnLayout {
            enabled: listView.currentIndex !== -1

            Layout.preferredWidth: 400
            Layout.maximumWidth: 400

            GridLayout {
                Layout.fillWidth: true
//                Layout.preferredWidth: 300
//                Layout.maximumWidth: 300

                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
                    enabled: false
                    Layout.fillWidth: true
                    value: listView.currentData ? listView.currentData.id : -1
                    onValueModified: listView.currentData.id = value
                }
                Label { text: qsTr("Name:") }
                TextField {
                    Layout.fillWidth: true
                    text: listView.currentData ? listView.currentData.name : ''
                    onTextEdited: listView.currentData.name = text
                }
                Label { text: qsTr("DeviceType:") }
                IconComboBox {
                    id: deviceTypeCombobox

                    Layout.fillWidth: true
                    Layout.preferredHeight: 64

                    model: DeviceTypesModel {
                        controller: __controller
                    }
                    textRole: "name"
                    valueRole: "id"
                    iconSourceRole: "iconUrl"
                    iconSourceRoleInt: DeviceTypesModel.IconUrlRole
                    currentIndex: listView.currentData ? deviceTypeCombobox.indexOfValue(listView.currentData.deviceTypeId) : -1
                    onActivated: if (listView.currentData) listView.currentData.deviceTypeId = currentValue; else console.warn('discarded');
                }
                Label { text: qsTr("Address:") }
                SpinBox {
                    to: 512
                    Layout.fillWidth: true
                    value: listView.currentData ? listView.currentData.address : -1
                    onValueModified: listView.currentData.address = value
                }
                Label { text: qsTr("Position:") }
                Vector3DField {
                    id: positionField
                    Layout.fillWidth: true
                    onValueModified: listView.currentData.position = value;
                    // TODO solve without onCurrentDataChanged
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }

        Devices3dView {
            id: devices3dView

            Layout.fillWidth: true
            Layout.fillHeight: true
//            Layout.preferredWidth: 300
            Layout.minimumWidth: 300

            model: model
            selectedItem: listView.currentIndex
            onSelectedItemChanged: if (selectedItem != -1) listView.currentIndex = selectedItem;
        }
    }
}
