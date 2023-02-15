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
                Layout.preferredWidth: 300
                Layout.maximumWidth: 300

                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
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
                Pane {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    EditableListView {
                        textRole: 'registerTypeName'

                        model: DeviceTypeRegistersModel {
                            controller: __controller
                            deviceTypeId: listView.currentData.id
                        }

                        anchors.fill: parent
                    }
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
