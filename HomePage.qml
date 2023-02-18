import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import com.büro 1.0

Item {
    id: homePage

    property alias masterDimmer: masterDimmer.value
    property alias masterRed: masterRed.value
    property alias masterGreen: masterGreen.value
    property alias masterBlue: masterBlue.value
    property alias masterWhite: masterWhite.value

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
                id: masterDimmer
            }

            Label {
                text: qsTr("Alle Rot:")
            }

            DmxSlider {
                id: masterRed
            }

            Label {
                text: qsTr("Alle Grün:")
            }

            DmxSlider {
                id: masterGreen
            }

            Label {
                text: qsTr("Alle Blau:")
            }

            DmxSlider {
                id: masterBlue
            }

            Label {
                text: qsTr("Alle Weiß:")
            }

            DmxSlider {
                id: masterWhite
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
