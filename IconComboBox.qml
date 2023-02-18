import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

ComboBox {
    id: comboBox

    property string iconSourceRole

    delegate: ItemDelegate {
        height: 64
        anchors.left: parent.left
        anchors.right: parent.right
        contentItem: IconChooserDelegateLayout {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            text: model[comboBox.textRole]
            iconSource: model[comboBox.iconSourceRole]
        }
    }
    contentItem: IconChooserDelegateLayout {
        text: comboBox.currentText
        iconSource: {
            if (!comboBox.model)
                return '';
            const url = comboBox.model.get(comboBox.currentIndex, "fileUrl");
            if (!url)
                return '';
            return url;
        }
    }
}
