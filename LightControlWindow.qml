import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.VirtualKeyboard

import com.büro 1.0

ApplicationWindow {
    id: window
    width: 1360
    height: 768
    title: qsTr("Hello World")

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    DeviceTypesModel {
        id: deviceTypesModel
        controller: __controller
    }

    DevicesModel {
        id: devicesModel
        controller: __controller
    }

    ColumnLayout {
        anchors.fill: parent

        StatusBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 75
        }

        StackView {
            id: stackview
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: homePage

            Component {
                id: homePage

                HomePage {
                }
            }

            pushEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 200
                }
            }
            pushExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to:0
                    duration: 200
                }
            }
            popEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to:1
                    duration: 200
                }
            }
            popExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to:0
                    duration: 200
                }
            }
        }
    }

    Button {
        id: closeButton

        visible: inputPanel.active

        anchors.bottom: inputPanel.top
        anchors.right: parent.right
        z: 99

        //onClicked: inputPanel.active = false
//        onClicked: InputContext.priv.hideInputPanel()

        text: qsTr("Close keyboard?")

        focus: false
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
            }
        }
        transitions: [
            Transition {
                from: "visible"
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
                to: "visible"
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
    }
}
