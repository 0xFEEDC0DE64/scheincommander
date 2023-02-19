import QtQuick

import lightcontrol

ListModel {
    ListElement { text: qsTr('Dummy');   value: DeviceTypeRegisterType.Dummy }
    ListElement { text: qsTr('Dimmer');  value: DeviceTypeRegisterType.Dimmer }
    ListElement { text: qsTr('Red');     value: DeviceTypeRegisterType.Red }
    ListElement { text: qsTr('Green');   value: DeviceTypeRegisterType.Green }
    ListElement { text: qsTr('Blue');    value: DeviceTypeRegisterType.Blue }
    ListElement { text: qsTr('White');   value: DeviceTypeRegisterType.White }
    ListElement { text: qsTr('Strobo');  value: DeviceTypeRegisterType.Strobo }
    ListElement { text: qsTr('Shutter'); value: DeviceTypeRegisterType.Shutter }
}
