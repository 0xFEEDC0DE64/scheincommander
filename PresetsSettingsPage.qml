import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

import scheincommander

ColumnLayout {
    property bool needsRegler: true

    Label {
        text: qsTr("Presets Settings")
    }
    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        EditableListView {
            id: presetsListView

            Layout.fillWidth: true
            Layout.fillHeight: true

            model: PresetsModel {
                id: presetsModel
                controller: __controller
            }

            onAddClicked: (index) => { const newIndex = index < 0 ? 0 : index + 1; if (presetsModel.insertRow(newIndex)) currentIndex = newIndex; else console.warn('failed'); }
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

                    onAccepted: if (!presetsModel.removeRow(index)) console.warn('failed');

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

            enabled: presetsListView.currentIndex !== -1

            GridLayout {
                columns: 2

                Label { text: qsTr("Id:") }
                SpinBox {
                    enabled: false
                    value: presetsListView.currentData ? presetsListView.currentData.id : -1
                    onValueModified: if (presetsListView.currentData) presetsListView.currentData.id = value; else console.warn('discarded');
                }
                Label { text: qsTr("Name:") }
                TextField {
                    Layout.fillWidth: true
                    text: presetsListView.currentData ? presetsListView.currentData.name : ''
                    onTextEdited: if (presetsListView.currentData) presetsListView.currentData.name = text; else console.warn('discarded');
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }

        EditableListView {
            Layout.fillWidth: true

            enabled: presetsListView.currentIndex !== -1

            model: PresetStepsModel {
                id: presetStepsModel

                controller: __controller
                presetId: presetsListView.currentData ? presetsListView.currentData.id : -1
            }

            onAddClicked: (index) => { const newIndex = index < 0 ? 0 : index + 1; if (presetStepsModel.insertRow(newIndex)) currentIndex = newIndex; else console.warn('failed'); }
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

                    onAccepted: if (!presetStepsModel.removeRow(index)) console.warn('failed');

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
            Layout.fillWidth: true

            PresetModel {
                id: presetModel
                controller: __controller
                presetId: presetsListView.currentData ? presetsListView.currentData.id : -1
            }

            RowLayout {
                Button {
                    text: qsTr('Auf Schieberegler\nunten kopieren');
                    onPressed: presetModel.copyToFaders()
                }
                Button {
                    text: qsTr('Von Schieberegler\nunten kopieren');
                    onPressed: presetModel.copyFromFaders()
                }
            }

            RowLayout {
                Button {
                    text: qsTr('Alle auf\n0 setzen');
                    onPressed: presetModel.setAllFadersLow()
                }
                Button {
                    text: qsTr('Alle auf\nMaximum setzen');
                    onPressed: presetModel.setAllFadersMax()
                }
            }

            RowLayout {
                SpinBox {
                    id: nSpinBox
                    Layout.preferredWidth: 120
                    from: 1
                }

                SpinBox {
                    id: kSpinBox
                    Layout.preferredWidth: 120

                    from: 0
                    to: nSpinBox.value - 1
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
                    onPressed: presetModel.setPattern(nSpinBox.value, kSpinBox.value, registerTypeComboBox.currentValue, valueSlider.value)
                }
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }
}
