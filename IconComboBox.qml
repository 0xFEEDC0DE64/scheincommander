import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.labs.folderlistmodel 2.4

ComboBox {
    id: comboBox

    property string iconSourceRole

    property bool hasTwoArgumentGetter: model instanceof FolderListModel

    function getIconUrl(index) {
        if (!comboBox.model)
            return '';
        if (hasTwoArgumentGetter)
            return model.get(index, iconSourceRole);
        else
            return model.get(index)[iconSourceRole];
    }

    delegate: ItemDelegate {
        height: 64
        anchors.left: parent.left
        anchors.right: parent.right
        contentItem: IconChooserDelegateLayout {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            text: model[comboBox.textRole]
            iconSource: comboBox.getIconUrl(index)
        }
    }
    contentItem: IconChooserDelegateLayout {
        text: comboBox.displayText
        iconSource: comboBox.currentIndex >= 0 ? comboBox.getIconUrl(comboBox.currentIndex) : ""
        isInsideMaterialComboBox: true
    }
}
