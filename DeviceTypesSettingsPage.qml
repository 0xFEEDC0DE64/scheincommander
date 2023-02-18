import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import com.büro 1.0

ColumnLayout {
    Label {
        text: qsTr("Device Types Settings")
    }

    RowLayout {
        //Layout.fillWidth: true
        Layout.fillHeight: true

        EditableListView {
            id: listView

            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            model: deviceTypesModel

            onAddClicked: (index) => deviceTypesModel.insertRow(index < 0 ? 0 : index + 1);
            onRemoveClicked: (index) => deviceTypesModel.removeRow(index)
        }

        ColumnLayout {
            enabled: listView.currentIndex !== -1

            GridLayout {
                Layout.preferredWidth: 600
                Layout.maximumWidth: 600

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
                Label { text: qsTr("Registers:") }
                RegistersSettingsItem {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    deviceTypeId: listView.currentData.id
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
