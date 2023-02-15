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

    Flickable {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        height: 300

        contentWidth: theFlow.width
        contentHeight: theFlow.height

        flickableDirection: Flickable.HorizontalFlick

        RowLayout {
            id: theFlow

            height: parent.height

            spacing: 5

            Repeater {
                model: devicesModel

                delegate: LightSliderPane {
                    light: model

                    //Layout.fillHeight: true

                    height: theFlow.height
                }
            }
        }
    }
}
