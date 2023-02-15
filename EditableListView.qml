import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property alias currentItem: listView.currentItem

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
        spacing: 2

        delegate: Rectangle {
            property variant myData: model

            width: listView.width
            height: 40

            color: ListView.isCurrentItem ?
                       Material.color(Material.Purple) :
                       Material.color(Material.Purple, Material.Shade900)
            radius: 5

            Label {
                anchors.fill: parent
                //anchors.verticalCenter: parent.verticalCenter
                id: text
                text: model.name
                padding: 5
                fontSizeMode: Text.VerticalFit
                minimumPixelSize: 10;
                font.pixelSize: 72
            }

            MouseArea {
                anchors.fill: parent
                onClicked: listView.currentIndex = index
            }
        }
        focus: true
    }
}
