import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

RowLayout {
    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Flow {
            anchors.centerIn: parent

            Repeater {
                model: devicesModel

                ColumnLayout {
                    width: 50
                    height: 200

                    Label {
                        Layout.fillWidth: true
                        text: index
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Slider {
                        id: slider
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        orientation: Qt.Vertical
                        from: 0
                        to: 255

                        onValueChanged: __controller.setChannel(32 + index, value)
                    }

                    Label {
                        Layout.fillWidth: true
                        text: Math.round(slider.value)
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }

    ColumnLayout {
        Layout.preferredWidth: 100
        Layout.fillHeight: true

        Button {
            Layout.preferredHeight: 100

            text: qsTr('Settings')

            onClicked: stackview.push(settingsPage)

            Component {
                id: settingsPage

                SettingsPage {
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
