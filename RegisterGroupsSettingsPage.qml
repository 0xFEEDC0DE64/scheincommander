import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import lightcontrol

ColumnLayout {
    property bool needsRegler: true

    Label {
        text: qsTr("Register Groups Settings")
    }
    RowLayout {
        //Layout.fillWidth: true
        Layout.fillHeight: true

        EditableListView {
            id: listView

            Layout.preferredWidth: 300
            Layout.maximumWidth: 300
            Layout.fillHeight: true

            model: RegisterGroupsModel {
                id: model
                controller: __controller
            }

            onAddClicked: (index) => { const newIndex = index < 0 ? 0 : index + 1; if (model.insertRow(newIndex)) currentIndex = newIndex; else console.warn('failed'); }
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

            GridLayout {
                Layout.preferredWidth: 600
                Layout.maximumWidth: 600

                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
                    enabled: false
                    Layout.fillWidth: true
                    value: listView.currentData ? listView.currentData.id : -1
                    onValueModified: if (listView.currentData) listView.currentData.id = value; else console.warn('discarded');
                }
                Label { text: qsTr("Name:") }
                TextField {
                    Layout.fillWidth: true
                    text: listView.currentData ? listView.currentData.name : ''
                    onTextEdited: if (listView.currentData) listView.currentData.name = text; else console.warn('discarded');
                }
            }
            Item {
                Layout.fillHeight: true
            }
        }
    }
}
