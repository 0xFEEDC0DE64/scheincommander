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

            onAddClicked: (index) => console.log('added', index)
            onRemoveClicked: (index) => devicesModel.removeRow(index)
        }

        ColumnLayout {
            GridLayout {
                Layout.preferredWidth: 300
                Layout.maximumWidth: 300

                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
                    Layout.fillWidth: true
                    value: listView.currentItem.myData.id
                    onValueModified: listView.currentItem.myData.id = value
                }
                Label { text: qsTr("Name:") }
                TextField {
                    Layout.fillWidth: true
                    text: listView.currentItem.myData.name
                    onTextEdited: listView.currentItem.myData.name = text
                }
                Label { text: qsTr("DeviceType:") }
                ComboBox {
                    Layout.fillWidth: true
                    model: deviceTypesModel
                    textRole: "name"
                    valueRole: "id"
                    // TODO
                }
                Label { text: qsTr("Address:") }
                SpinBox {
                    Layout.fillWidth: true
                    value: listView.currentItem.myData.address
                    onValueModified: listView.currentItem.myData.address = value
                }
                Label { text: qsTr("Position:") }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
