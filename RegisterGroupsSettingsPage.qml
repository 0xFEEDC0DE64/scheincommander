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
                Layout.preferredWidth: 300
                Layout.maximumWidth: 300

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

            GridLayout {
                Layout.fillWidth: true

                columns: 3

                RegisterGroupModel {
                    id: registerGroupModel
                    controller: __controller
                    registerGroupId: listView.currentData ? listView.currentData.id : -1
                }

                Button {
                    text: qsTr('Auf Schieberegler\nunten kopieren');
                    onPressed: registerGroupModel.copyToFaders()
                }
                Button {
                    text: qsTr('Von Schieberegler\nunten kopieren');
                    onPressed: registerGroupModel.copyFromFaders()
                }
                Item {
                    Layout.rowSpan: 2
                    Layout.fillWidth: true
                }
                Button {
                    text: qsTr('Alle auf\n0 setzen');
                    onPressed: registerGroupModel.setAllFadersLow()
                }
                Button {
                    text: qsTr('Alle auf\nMaximum setzen');
                    onPressed: registerGroupModel.setAllFadersMax()
                }
                RowLayout {
                    Layout.columnSpan: 3

                    SpinBox {
                        id: nSpinBox
                        Layout.preferredWidth: 120
                        from: 1
                    }

                    SpinBox {
                        id: kSpinBox
                        Layout.preferredWidth: 120
                    }

                    ComboBox {
                        id: registerTypeComboBox
                        model: DeviceTypeRegisterTypesModel {
                        }
                        textRole: "text"
                        valueRole: "value"
                    }

                    DmxSlider {
                        id: valueSlider
                    }

                    Button {
                        text: qsTr('Set')
                        onPressed: registerGroupModel.setPattern(nSpinBox.value, kSpinBox.value, registerTypeComboBox.currentValue, valueSlider.value)
                    }
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
