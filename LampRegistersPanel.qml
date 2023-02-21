import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import scheincommander

Flickable {
    id: lampRegistersPanel

    property bool active

    states: State {
        name: "invisible"
        when: !lampRegistersPanel.active
        PropertyChanges {
            target: lampRegistersPanel
            y: window.height
        }
    }
    transitions: [
        Transition {
            from: "invisible"
            to: ""
            reversible: false
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }
        },
        Transition {
            from: ""
            to: "invisible"
            reversible: false
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }
        }
    ]

    contentWidth: theFlow.width
    contentHeight: theFlow.height

    flickableDirection: Flickable.HorizontalFlick

    RowLayout {
        id: theFlow

        height: parent.height

        spacing: 5

        Repeater {
            model: DevicesModel {
                controller: __controller
            }

            delegate: LightSliderPane {
                light: model

                //Layout.fillHeight: true

                height: theFlow.height
            }
        }
    }
}
