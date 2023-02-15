import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

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

            onAddClicked: (index) => console.log('added', index);
            onRemoveClicked: (index) => deviceTypesModel.removeRow(index)
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
                Label { text: qsTr("Registers:") }
                Pane {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    EditableListView {
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
