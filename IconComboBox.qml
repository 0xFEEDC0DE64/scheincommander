import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.labs.folderlistmodel 2.4

import scheincommander 1.0

ComboBox {
    id: comboBox

    property string iconSourceRole
    property int iconSourceRoleInt

    property string iconSource: comboBox.currentIndex === -1
                                ? ""
                                : Array.isArray(comboBox.model)
                                    ? comboBox.model[comboBox.currentIndex][comboBox.iconSourceRole]
                                    : (comboBox.model as ListModel)?.get(comboBox.currentIndex)[comboBox.iconSourceRole]
                                        ?? (comboBox.model as FolderListModel)?.get(comboBox.currentIndex, comboBox.iconSourceRole)
                                        ?? comboBox.model.data(comboBox.model.index(comboBox.currentIndex, 0), comboBox.iconSourceRoleInt)

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
        text: comboBox.displayText
        iconSource: comboBox.iconSource
        isInsideMaterialComboBox: true
    }
}
