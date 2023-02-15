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

                    Slider {
                        id: slider
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        orientation: Qt.Vertical
                        from: 0
                        to: 255

                        snapMode: Slider.SnapAlways
                        stepSize: 1

                        onValueChanged: __controller.setChannel(registerAddress, value)
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
