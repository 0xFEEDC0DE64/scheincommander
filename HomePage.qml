import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import com.büro 1.0

RowLayout {
    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true

        Flow {
            anchors.fill: parent

            spacing: 5

            Repeater {
                model: devicesModel

                delegate: Pane {
                    property int deviceAddress: model.address
                    property int myDeviceTypeId: model.deviceTypeId

                    Material.elevation: 6

                    //width: 75
                    height: 250

                    ColumnLayout {
                        anchors.fill: parent

                        Label {
                            Layout.fillWidth: true
                            text: model.name
                            horizontalAlignment: Text.AlignHCenter
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            Repeater {
                                model: DeviceTypeRegistersModel {
                                    controller: __controller
                                    deviceTypeId: myDeviceTypeId
                                }

                                delegate: ColumnLayout {
                                    property int registerAddress: deviceAddress + index

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
            }
        }
    }

    ColumnLayout {
        Layout.preferredWidth: 100
        Layout.fillHeight: true

        Button {
            Layout.preferredHeight: 100

            text: qsTr('Settings')

            onClicked: stackview.push(settingsPage)

            Component {
                id: settingsPage

                SettingsPage {
                }
            }
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
