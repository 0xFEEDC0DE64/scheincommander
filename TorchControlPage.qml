import QtQuick

import scheincommander

Item {
    Rectangle {
        id: torchArea

        anchors.centerIn: parent
        width: 500
        height: 500

        Repeater {
            model: DevicesModel {
                controller: __controller
            }

            delegate: Rectangle {
                width: 10
                height: 10
                x: (torchArea.width / 2) + (model.position.x / 10 * torchArea.width / 2) - (width / 2)
                y: (torchArea.height / 2) + (model.position.z / 10 * torchArea.height / 2) - (height / 1)

                color: Qt.rgba(255, 0, 0)
                radius: width/2
            }
        }

        MultiPointTouchArea {
            anchors.fill: parent

            touchPoints: [
                TouchPoint { id: point1 },
                TouchPoint { id: point2 }
            ]
        }

        Rectangle {
            width: 30; height: 30
            color: "green"
            x: point1.x
            y: point1.y
        }

        Rectangle {
            width: 30; height: 30
            color: "yellow"
            x: point2.x
            y: point2.y
        }
    }
}
