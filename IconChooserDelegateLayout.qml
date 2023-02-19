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
        Layout.preferredHeight: layout.height
        text: layout.text
        verticalAlignment: Label.AlignVCenter
        fontSizeMode: Text.VerticalFit
        font.pixelSize: 20
    }
    Item {
        Layout.fillWidth: true
    }
}
