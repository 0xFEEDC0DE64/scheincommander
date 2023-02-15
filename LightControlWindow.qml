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

        Pane {
            Layout.fillWidth: true
            Layout.preferredHeight: 75

            Material.elevation: 6

            z: 999

            RowLayout {
                anchors.fill: parent

                Label {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: qsTr("Schein-Commander")
                    fontSizeMode: Text.VerticalFit
                    minimumPixelSize: 10;
                    font.pixelSize: 72
                }

                Label {
                    id: timeText
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: Qt.formatTime(new Date(), "hh:mm:ss")
                    fontSizeMode: Text.VerticalFit
                    minimumPixelSize: 10;
                    font.pixelSize: 72

                    Timer {
                        id: timer
                        interval: 1000
                        repeat: true
                        running: true

                        onTriggered: timeText.text = Qt.formatTime(new Date(), "hh:mm:ss");
                    }
                }

                Button {
                    Layout.fillHeight: true

                    text: qsTr("Back")

                    onClicked: stackview.pop();
                    enabled: stackview.depth > 1
                }
            }
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
