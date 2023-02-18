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
            id: deviceTypesListView

            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            model: deviceTypesModel

            onAddClicked: (index) => deviceTypesModel.insertRow(index < 0 ? 0 : index + 1);
            onRemoveClicked: (index) => deviceTypesModel.removeRow(index)
        }

        ColumnLayout {
            enabled: deviceTypesListView.currentIndex !== -1

            GridLayout {
                Layout.preferredWidth: 600
                Layout.maximumWidth: 600

                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
                    enabled: false
                    Layout.fillWidth: true
                    value: deviceTypesListView.currentData.id
                    onValueModified: deviceTypesListView.currentData.id = value
                }
                Label { text: qsTr("Name:") }
                TextField {
                    Layout.fillWidth: true
                    text: deviceTypesListView.currentData.name
                    onTextEdited: deviceTypesListView.currentData.name = text
                }
                Label { text: qsTr("Registers:") }
                Pane {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Material.elevation: 6

                    RowLayout {
                        anchors.fill: parent

                        Pane {
                            Layout.preferredWidth: 300
                            Layout.fillHeight: true

                            Material.elevation: 6

                            EditableListView {
                                id: deviceTypesRegistersListView
                                anchors.fill: parent

                                textRole: 'registerTypeName'

                                model: DeviceTypeRegistersModel {
                                    controller: __controller
                                    deviceTypeId: deviceTypesListView.currentData.id
                                }
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            enabled: deviceTypesRegistersListView.currentIndex >= 0

                            GridLayout {
                                Layout.fillWidth: true

                                columns: 2

                                Label {
                                    text: qsTr('Type:')
                                }
                                ComboBox {
                                    id: comboBox
                                    model: deviceTypeRegisterTypesModel
                                    textRole: "text"
                                    valueRole: "value"

                                    currentIndex: deviceTypesRegistersListView.currentData ? comboBox.indexOfValue(deviceTypesRegistersListView.currentData.registerType) : -1
                                }
                            }

                            Item {
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
