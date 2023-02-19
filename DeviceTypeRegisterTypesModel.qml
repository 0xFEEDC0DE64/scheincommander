import QtQuick

import lightcontrol

ListModel {
    ListElement { text: qsTr('Dummy');    value: DeviceTypeRegisterType.Dummy }
    ListElement { text: qsTr('Dimmer');   value: DeviceTypeRegisterType.Dimmer }
    ListElement { text: qsTr('Red');      value: DeviceTypeRegisterType.Red }
    ListElement { text: qsTr('Green');    value: DeviceTypeRegisterType.Green }
    ListElement { text: qsTr('Blue');     value: DeviceTypeRegisterType.Blue }
    ListElement { text: qsTr('White');    value: DeviceTypeRegisterType.White }
    ListElement { text: qsTr('Shutter');  value: DeviceTypeRegisterType.Shutter }
    ListElement { text: qsTr('Pan');      value: DeviceTypeRegisterType.Pan }
    ListElement { text: qsTr('PanFine');  value: DeviceTypeRegisterType.PanFine }
    ListElement { text: qsTr('Tilt');     value: DeviceTypeRegisterType.Tilt }
    ListElement { text: qsTr('TiltFine'); value: DeviceTypeRegisterType.TiltFine }
    ListElement { text: qsTr('Color');    value: DeviceTypeRegisterType.Color }
    ListElement { text: qsTr('Cyan');     value: DeviceTypeRegisterType.Cyan }
    ListElement { text: qsTr('Magenta');  value: DeviceTypeRegisterType.Magenta }
    ListElement { text: qsTr('Yellow');   value: DeviceTypeRegisterType.Yellow }
    ListElement { text: qsTr('Gobo');     value: DeviceTypeRegisterType.Gobo }
    ListElement { text: qsTr('Rotation'); value: DeviceTypeRegisterType.Rotation }
    ListElement { text: qsTr('Prism');    value: DeviceTypeRegisterType.Prism }
    ListElement { text: qsTr('Frost');    value: DeviceTypeRegisterType.Frost }
    ListElement { text: qsTr('Focus');    value: DeviceTypeRegisterType.Focus }
    ListElement { text: qsTr('Speed');    value: DeviceTypeRegisterType.Speed }
    ListElement { text: qsTr('Iris');     value: DeviceTypeRegisterType.Iris }
    ListElement { text: qsTr('Zoom');     value: DeviceTypeRegisterType.Zoom }
    ListElement { text: qsTr('Extra1');   value: DeviceTypeRegisterType.Extra1 }
    ListElement { text: qsTr('Extra2');   value: DeviceTypeRegisterType.Extra2 }
}
