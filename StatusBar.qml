import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Pane {
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

                onTriggered: parent.text = Qt.formatTime(new Date(), "hh:mm:ss");
            }
        }

        Label {
            Layout.fillWidth: true
            Layout.fillHeight: true

            text: __controller.performance + ' FPS'
        }

        Button {
            Layout.fillHeight: true

            text: qsTr("Back")

            onClicked: stackview.pop();
            enabled: stackview.depth > 1
        }
    }
}
