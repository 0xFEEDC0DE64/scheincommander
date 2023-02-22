import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

ColumnLayout {
    property string textRole: "name"
    property string iconSourceRole

    property alias model: listView.model
    property alias currentIndex: listView.currentIndex
    property alias currentItem: listView.currentItem
    property variant currentData: listView.currentItem ? listView.currentItem.myData : null

    signal addClicked(index: int)
    signal removeClicked(index: int)

    id: editableListViewLayout

    RowLayout {
        Layout.fillWidth: true

        Button {
            text: "\ue145"
            font.family: materialIcons.font.family
            font.pixelSize: 20

            onClicked: addClicked(listView.currentIndex)
        }

        Item {
            Layout.fillWidth: true
        }

        Button {
            text: "\ue872"
            font.family: materialIcons.font.family
            font.pixelSize: 20

            onClicked: removeClicked(listView.currentIndex)
            enabled: listView.currentIndex >= 0
        }
    }

    ListView {
        id: listView

        Layout.fillWidth: true
        Layout.fillHeight: true

        clip: true
        focus: true
//        highlightRangeMode: ListView.ApplyRange

        onCountChanged: if (count === 0 && currentIndex >= 0) currentIndex = -1

//        displaced: Transition {
//            NumberAnimation { properties: "x,y"; duration: 1000 }
//        }

        addDisplaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }
        add: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 1000 }
                NumberAnimation { properties: "x"; from: 100; duration: 1000 }
            }
        }

        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }
        moveDisplaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }

//        populate: Transition {
//            NumberAnimation { properties: "x,y"; duration: 100 }
//        }

        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                NumberAnimation { properties: "x"; to: 100; duration: 1000 }
            }
        }
        removeDisplaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }

        delegate: Item {
            property variant myData: model
            readonly property bool isCurrentItem: ListView.isCurrentItem

            width: listView.width
            height: 45

            ItemDelegate {
                anchors.fill: parent
                contentItem: IconChooserDelegateLayout {
                    anchors.fill: parent
                    text: model[textRole]
                    iconSource: (model && model[iconSourceRole]) ? model[iconSourceRole] : null
                }
                onClicked: listView.currentIndex = index
                down: listView.currentIndex === index
            }

            Rectangle {
                color: Material.dividerColor
                height: 1
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                visible: index !== (listView.count - 1)
            }
        }
    }
}
