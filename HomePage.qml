import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import com.büro 1.0

RowLayout {
    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Flow {
            anchors.fill: parent

            spacing: 5

            Repeater {
                model: devicesModel

                delegate: LightSliderPane {
                    light: model

                    height: 250
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
