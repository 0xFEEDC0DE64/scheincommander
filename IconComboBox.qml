import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt.labs.folderlistmodel 2.4

import scheincommander 1.0

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
        text: comboBox.displayText
        iconSource: {
//            console.log("QAbstractListModel", model instanceof QAbstractListModel);
//            console.log("QAbstractItemModel", model instanceof QAbstractItemModel);
//            console.log("FolderListModel", model instanceof FolderListModel);
//            console.log("DeviceTypesModel", model instanceof DeviceTypesModel);
//            console.log("QtObject", model instanceof QtObject);

            if (comboBox.currentIndex < 0)
                return '';
            if (!comboBox.model)
                return '';
            if (!comboBox.iconSourceRole)
                return '';

            if (model instanceof FolderListModel)
                return model.get(comboBox.currentIndex, iconSourceRole);
            else if ('get' in model)
            {
                const data = model.get(comboBox.currentIndex);
                console.log(data);
                return data[iconSourceRole];
            }
            else if ('roleNames' in model || 'itemData' in model)
            {
                if (!('roleNames' in model && 'itemData' in model))
                    throw 'roleNames or itemData not defined!';

                const roleNames = model.roleNames();
                console.log('roleNames', roleNames);

                const index = model.index(comboBox.currentIndex, 0);
                const itemData = model.itemData(index);
                console.log('itemData', itemData);

                throw 'getting data from model using roleNames and itemData is not yet implemented.';
            }
            else
                throw 'unknown model type' + typeof model;
        }
        isInsideMaterialComboBox: true
    }
}
