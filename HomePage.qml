import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import scheincommander

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

    Flow{
        anchors.fill: parent
        flow: Flow.TopToBottom

        Repeater {
            model: PresetsModel {
                controller: __controller
            }

            delegate: RowLayout {
                Label {
                    text: model.name
                }
                DmxSlider {
                    onValueChanged: __controller.setPresetSlider(model.id, value);
                }
            }
        }
        CheckBox {
            id: animationCheckbox
            text: qsTr('Animation')
        }
    }

    property int test0
    onTest0Changed: __controller.setPresetSlider(7, test0);
    property int test1
    onTest1Changed: __controller.setPresetSlider(15, test1);
    property int test2
    onTest2Changed: __controller.setPresetSlider(9, test2);
    property int test3
    onTest3Changed: __controller.setPresetSlider(13, test3);

    SequentialAnimation {
        loops: Animation.Infinite
        running: animationCheckbox.checked

        ParallelAnimation {
            NumberAnimation { target: homePage; property: "test0"; duration: 2000; from: 10; to: 255; easing.type: Easing.InQuad }
            NumberAnimation { target: homePage; property: "test1"; duration: 2000; from: 255; to: 10; easing.type: Easing.InQuad } // blau
            NumberAnimation { target: homePage; property: "test2"; duration: 2000; from: 255; to: 10; easing.type: Easing.InQuad } // blau
            NumberAnimation { target: homePage; property: "test3"; duration: 2000; from: 10; to: 255; easing.type: Easing.InQuad }
        }
        PauseAnimation { duration: 1000 }
        ParallelAnimation {
            NumberAnimation { target: homePage; property: "test0"; duration: 2000; from: 255; to: 10; easing.type: Easing.InQuad }
            NumberAnimation { target: homePage; property: "test1"; duration: 2000; from: 10; to: 255; easing.type: Easing.InQuad } // blau
            NumberAnimation { target: homePage; property: "test2"; duration: 2000; from: 10; to: 255; easing.type: Easing.InQuad } // blau
            NumberAnimation { target: homePage; property: "test3"; duration: 2000; from: 255; to: 10; easing.type: Easing.InQuad }
        }
        PauseAnimation { duration: 1000 }
    }
}
