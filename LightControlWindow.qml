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

    ListModel {
        id: deviceTypeRegisterTypesModel
        ListElement { text: qsTr('Dimmer');  value: DeviceTypeRegisterType.Dimmer }
        ListElement { text: qsTr('Red');     value: DeviceTypeRegisterType.Red }
        ListElement { text: qsTr('Green');   value: DeviceTypeRegisterType.Green }
        ListElement { text: qsTr('Blue');    value: DeviceTypeRegisterType.Blue }
        ListElement { text: qsTr('White');   value: DeviceTypeRegisterType.White }
        ListElement { text: qsTr('Strobo');  value: DeviceTypeRegisterType.Strobo }
        ListElement { text: qsTr('Shutter'); value: DeviceTypeRegisterType.Shutter }
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

    Flickable {
        id: test

        z: 98
        x: 0
        height: 300
        y: window.height - height
        width: window.width
        property bool active: typeof stackview.currentItem.needsRegler == 'boolean' ? stackview.currentItem.needsRegler : false

        states: State {
            name: "invisible"
            when: !test.active
            PropertyChanges {
                target: test
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
                model: devicesModel

                delegate: LightSliderPane {
                    light: model

                    //Layout.fillHeight: true

                    height: theFlow.height
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
