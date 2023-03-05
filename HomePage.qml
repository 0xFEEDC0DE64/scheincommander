import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import scheincommander

Item {
    id: homePage

    property bool needsRegler: true

    ColumnLayout {
        anchors.top: parent.top
        anchors.right: parent.right

        Button {
            width: 100
            height: 100
            Layout.minimumWidth: width
            Layout.minimumHeight: height
            Layout.maximumWidth: width
            Layout.maximumHeight: height

            text: qsTr('Settings')

            onClicked: stackview.push(settingsPage)

            Component {
                id: settingsPage

                SettingsPage {
                }
            }
        }

        Button {
            width: 100
            height: 100
            Layout.minimumWidth: width
            Layout.minimumHeight: height
            Layout.maximumWidth: width
            Layout.maximumHeight: height

            text: qsTr('Torch\nControl')

            onClicked: stackview.push(torchControlPage)

            Component {
                id: torchControlPage

                TorchControlPage {
                }
            }
        }
    }

    Flow {
        anchors.fill: parent
        flow: Flow.TopToBottom

        Repeater {
            model: PresetsModel {
                controller: __controller
            }

            delegate: RowLayout {
                Label {
                    text: model.name
                }
                DmxSlider {
                    onValueChanged: __controller.setPresetSlider(model.id, value);
                }
            }
        }
    }
}
