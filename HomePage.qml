import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import com.büro 1.0

Item {
    id: homePage

    property bool needsRegler: true

    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        width: 100
        height: 100

        text: qsTr('Settings')

        onClicked: stackview.push(settingsPage)

        Component {
            id: settingsPage

            SettingsPage {
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        GridLayout {
            Layout.fillWidth: true

            columns: 2

            Label {
                text: qsTr("Alle Dimmer:")
            }

            DmxSlider {
                onValueChanged: window.masterDimmer = value
            }

            Label {
                text: qsTr("Alle Rot:")
            }

            DmxSlider {
                onValueChanged: window.masterRed = value
            }

            Label {
                text: qsTr("Alle Grün:")
            }

            DmxSlider {
                onValueChanged: window.masterGreen = value
            }

            Label {
                text: qsTr("Alle Blau:")
            }

            DmxSlider {
                onValueChanged: window.masterBlue = value
            }

            Label {
                text: qsTr("Alle Weiß:")
            }

            DmxSlider {
                onValueChanged: window.masterWhite = value
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
