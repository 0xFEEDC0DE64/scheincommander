import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

RowLayout {
    property string text
    property string iconSource

    id: layout

    Image {
        Layout.topMargin: 15
        Layout.bottomMargin: 15
        Layout.fillHeight: true
        source: layout.iconSource
        fillMode: Image.PreserveAspectFit
    }
    Label {
        text: layout.text
    }
    Item {
        Layout.fillWidth: true
    }
}
