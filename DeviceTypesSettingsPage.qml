import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import scheincommander

ColumnLayout {
    Label {
        text: qsTr("Device Types Settings")
    }
    RowLayout {
        //Layout.fillWidth: true
        Layout.fillHeight: true

        EditableListView {
            id: deviceTypesListView

            iconSourceRole: "iconUrl"

            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            model: DeviceTypesModel {
                id: deviceTypesModel
                controller: __controller
            }

            onAddClicked: (index) => { const newIndex = index < 0 ? 0 : index + 1; if (deviceTypesModel.insertRow(newIndex)) currentIndex = newIndex; else console.warn('failed'); }
            onRemoveClicked: (index) => {
                const dialog = dialogComponent.createObject(Overlay.overlay);
                dialog.index = index;
                dialog.open();
            }

            Component {
                id: dialogComponent

                Dialog {
                    property int index

                    anchors.centerIn: parent
                    standardButtons: DialogButtonBox.Yes | DialogButtonBox.Cancel
                    modal: true
                    title: qsTr('Confirmation')

                    onAccepted: if (!deviceTypesModel.removeRow(index)) console.warn('failed');

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
            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            enabled: deviceTypesListView.currentIndex !== -1

            GridLayout {
                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
                    enabled: false
                    value: deviceTypesListView.currentData ? deviceTypesListView.currentData.id : -1
                    onValueModified: if (deviceTypesListView.currentData) deviceTypesListView.currentData.id = value; else console.warn('discarded');
                }
                Label { text: qsTr("Name:") }
                TextField {
                    Layout.fillWidth: true
                    text: deviceTypesListView.currentData ? deviceTypesListView.currentData.name : ''
                    onTextEdited: if (deviceTypesListView.currentData) deviceTypesListView.currentData.name = text; else console.warn('discarded');
                }
                Label { text: qsTr("Icon:") }
                IconComboBox {
                    id: iconComboBox

                    Layout.preferredHeight: 64

                    textRole: "fileBaseName"
                    valueRole: "fileBaseName"
                    iconSourceRole: "fileUrl"

                    model: IconsModel {
                        id: iconsModel
                    }

                    currentIndex: deviceTypesListView.currentData ? iconComboBox.indexOfValue(deviceTypesListView.currentData.iconName) : -1
                    Component.onCompleted: {
                        iconsModel.onRowCountChanged.connect(function(){
                            currentIndex = Qt.binding(function() { return deviceTypesListView.currentData ? iconComboBox.indexOfValue(deviceTypesListView.currentData.iconName) : -1});
                        });
                    }

                    onActivated: if (deviceTypesListView.currentData) deviceTypesListView.currentData.iconName = currentValue; else console.warn('discarded');
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }

        EditableListView {
            id: registersListView

            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            enabled: deviceTypesListView.currentIndex !== -1

            textRole: 'registerTypeName'

            model: DeviceTypeRegistersModel {
                id: registersModel
                controller: __controller

                deviceTypeId: deviceTypesListView.currentData ? deviceTypesListView.currentData.id : -1
            }

            onAddClicked: (index) => { const newIndex = index < 0 ? 0 : index + 1; if (registersModel.insertRow(newIndex)) currentIndex = newIndex; else console.warn('failed'); }
            onRemoveClicked: (index) => {
                const dialog = dialogComponent2.createObject(Overlay.overlay);
                dialog.index = index;
                dialog.open();
            }

            Component {
                id: dialogComponent2

                Dialog {
                    property int index

                    anchors.centerIn: parent
                    standardButtons: DialogButtonBox.Yes | DialogButtonBox.Cancel
                    modal: true
                    title: qsTr('Confirmation')

                    onAccepted: if (!registersModel.removeRow(index)) console.warn('failed');

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
            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            enabled: registersListView.currentIndex >= 0

            GridLayout {
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

                    currentIndex: registersListView.currentData ? comboBox.indexOfValue(registersListView.currentData.registerType) : -1
                    onActivated: if (registersListView.currentData) registersListView.currentData.registerType = currentValue; else console.warn('discarded');
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }

        Item {
            Layout.fillWidth: true
        }
    }
}
