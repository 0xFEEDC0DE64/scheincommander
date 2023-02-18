import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Qt.labs.folderlistmodel 2.4

import com.büro 1.0

ColumnLayout {
    FolderListModel {
        id: iconsModel
        folder: "qrc:/lightcontrol/icons/"
        showDirs: false
    }

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

                    onAccepted: deviceTypesModel.removeRow(index)

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

            GridLayout {
                Layout.preferredWidth: 600
                Layout.maximumWidth: 600

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
                Label { text: qsTr("Icon:") }
                ComboBox {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 64
                    id: iconCombobox
                    textRole: "fileName"
                    valueRole: "fileUrl"
                    delegate: ItemDelegate {
                        height: 64
                        anchors.left: parent.left
                        anchors.right: parent.right
                        contentItem: IconChooserDelegateLayout {
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            text: fileName
                            iconSource: fileUrl
                        }
                    }
                    contentItem: IconChooserDelegateLayout {
                        text: iconCombobox.currentText
                        iconSource: iconCombobox.currentValue
                    }

                    model: iconsModel
                }
                Label { text: qsTr("Registers:") }
                RegistersSettingsItem {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    deviceTypeId: listView.currentData ? listView.currentData.id : -1
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
