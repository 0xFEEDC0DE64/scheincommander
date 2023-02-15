import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.VirtualKeyboard

import com.büro 1.0

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

                    DmxSlider {
                        id: slider
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        orientation: Qt.Vertical

                        onValueChanged: __controller.setChannel(registerAddress, value)

                        Binding on value {
                            value: homePage.masterDimmer
                            when: model.registerType === DeviceTypeRegisterType.Dimmer
                        }
                        Binding on value {
                            value: homePage.masterRed
                            when: model.registerType === DeviceTypeRegisterType.Red
                        }
                        Binding on value {
                            value: homePage.masterGreen
                            when: model.registerType === DeviceTypeRegisterType.Green
                        }
                        Binding on value {
                            value: homePage.masterBlue
                            when: model.registerType === DeviceTypeRegisterType.Blue
                        }
                        Binding on value {
                            value: homePage.masterWhite
                            when: model.registerType === DeviceTypeRegisterType.White
                        }
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
