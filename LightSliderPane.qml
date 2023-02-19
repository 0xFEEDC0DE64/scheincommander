import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import lightcontrol

Pane {
    property variant light

    Material.elevation: 6

    ColumnLayout {
        anchors.fill: parent

        Label {
            Layout.fillWidth: true
            text: light.name
            horizontalAlignment: Text.AlignHCenter
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Repeater {
                model: DeviceTypeRegistersModel {
                    controller: __controller
                    deviceTypeId: light.deviceTypeId
                }

                delegate: ColumnLayout {
                    property int registerAddress: light.address + index

                    Layout.fillHeight: true

                    Label {
                        Layout.fillWidth: true
                        text: model.registerTypeName
                        horizontalAlignment: Text.AlignHCenter
                    }

                    DeviceRegisterValueHelper {
                        id: helper
                        controller: __controller
                        deviceId: light.id
                        registerIndex: index
                        value: slider.value
                    }

                    DmxSlider {
                        id: slider
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        value: helper.value

                        orientation: Qt.Vertical
                    }

                    Label {
                        Layout.fillWidth: true
                        text: Math.round(slider.value)
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
        }
    }
}
