import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    Label {
        text: qsTr("Devices Settings")
    }

    RowLayout {
        //Layout.fillWidth: true
        Layout.fillHeight: true

        EditableListView {
            id: listView

            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            model: devicesModel

            onAddClicked: (index) => devicesModel.insertRow(index < 0 ? 0 : index + 1);
            onRemoveClicked: (index) => devicesModel.removeRow(index)

            onCurrentDataChanged: if (currentData) test.value = currentData.position
        }

        ColumnLayout {
            enabled: listView.currentIndex !== -1

            GridLayout {
                Layout.preferredWidth: 300
                Layout.maximumWidth: 300

                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
                    enabled: false
                    Layout.fillWidth: true
                    value: listView.currentData.id
                    onValueModified: listView.currentData.id = value
                }
                Label { text: qsTr("Name:") }
                TextField {
                    Layout.fillWidth: true
                    text: listView.currentData.name
                    onTextEdited: listView.currentData.name = text
                }
                Label { text: qsTr("DeviceType:") }
                ComboBox {
                    id: deviceTypeCombobox
                    Layout.fillWidth: true
                    model: deviceTypesModel
                    textRole: "name"
                    valueRole: "id"
                    currentIndex: listView.currentData ? deviceTypeCombobox.indexOfValue(listView.currentData.deviceTypeId) : -1
                    onActivated: if (listView.currentData) listView.currentData.deviceTypeId = currentValue; else console.warn('discarded');
                }
                Label { text: qsTr("Address:") }
                SpinBox {
                    Layout.fillWidth: true
                    value: listView.currentData.address
                    onValueModified: listView.currentData.address = value
                }
                Label { text: qsTr("Position:") }
                Vector3DField {
                    id: test
                    Layout.fillWidth: true
                    onValueModified: listView.currentData.position = value;
                    // TODO solve without onCurrentDataChanged
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
