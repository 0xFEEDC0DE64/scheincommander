import QtQuick
import QtQuick.Controls.Material
import QtQuick.Dialogs
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

        Button {
            text: "\ue161"
            font.family: materialIcons.font.family
            font.pixelSize: 20
            onPressed: {
                if (typeof __controller.settings.lastProjectFile == "string" && __controller.settings.lastProjectFile !== "") {
                    console.log(__controller.settings.lastProjectFile);
                    if (!__controller.saveProject(__controller.settings.lastProjectFile)) {
                        console.warn('failed to save');
                    }
                } else {
                    fileDialog.open();
                }
            }

            FileDialog {
                id: fileDialog
                title: qsTr("Please select where to save the project file")
                fileMode: FileDialog.SaveFile
                onAccepted: {
                    if (!__controller.saveProject(fileDialog.selectedFile)) {
                        console.warn('failed to save');
                    }
                }
            }
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

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Label {
                text: __controller.dmxFps + ' FPS'
            }

            Label {
                text: __controller.computeMaxElapsed + "ms / " + __controller.dmxMaxElapsed + "ms"
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
