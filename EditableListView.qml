import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    property string textRole: "name"

    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property alias currentItem: listView.currentItem
    property variant currentData: listView.currentItem ? listView.currentItem.myData : null

    signal addClicked(index: int)
    signal removeClicked(index: int)

    RowLayout {
        Layout.fillWidth: true

        Button {
            text: qsTr("Add")

            onClicked: addClicked(listView.currentIndex)
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("Remove")

            onClicked: removeClicked(listView.currentIndex)
            enabled: listView.currentIndex >= 0
        }
    }

    ListView {
        id: listView

        Layout.fillWidth: true
        Layout.fillHeight: true

        clip: true

        delegate: Item {
            property variant myData: model
            readonly property bool isCurrentItem: ListView.isCurrentItem

            width: listView.width
            height: 45

            Rectangle {
                anchors.fill: parent

                color: isCurrentItem ?
                           Material.color(Material.Purple) :
                           Material.background
                radius: 0

                Label {
                    anchors.fill: parent
                    //anchors.verticalCenter: parent.verticalCenter
                    id: text
                    text: model[textRole]
                    padding: 10
                    fontSizeMode: Text.VerticalFit
                    minimumPixelSize: 10;
                    font.pixelSize: 72
                }
            }

            Rectangle {
                color: Material.dividerColor
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                visible: index !== (listView.count - 1)
            }

            MouseArea {
                anchors.fill: parent
                onClicked: listView.currentIndex = index
            }
        }
        focus: true
    }
}
