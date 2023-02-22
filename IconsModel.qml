import QtQuick
import Qt.labs.folderlistmodel 2.4

FolderListModel {
    folder: "qrc:/scheincommander/icons/"
    showDirs: false

    function getUrlForIcon(name) {
        let myFolder = folder;
        if (myFolder.length < 1 || myFolder.charAt(myFolder.length - 1) !== '/') {
            myFolder = myFolder + '/';
        }

        return myFolder + name + ".png"
    }
}
