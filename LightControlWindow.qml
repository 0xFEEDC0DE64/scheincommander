import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window

ApplicationWindow {
    id: window
    width: 1360
    height: 768
    title: qsTr("Hello World")

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    FontLoader {
        id: materialIcons
        source: "qrc:/scheincommander/ui-icons/MaterialIcons-Regular.ttf"
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: window.height - (lampRegistersPanel.active ? lampRegistersPanel.height : 0)

        StatusBar {
            Layout.fillWidth: true
            Layout.preferredHeight: 75
        }

        AnimatedStackView {
            id: stackview
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: Component {
                HomePage {
                }
            }
        }
    }

    LampRegistersPanel {
        id: lampRegistersPanel
        z: 98
        x: 0
        height: 300
        y: window.height - height
        width: window.width

        active: typeof stackview.currentItem.needsRegler == 'boolean' ? stackview.currentItem.needsRegler : false
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

    AnimatedInputPanel {
        id: inputPanel

        z: 99
        x: 0
        y: window.height
        width: window.width
    }
}
