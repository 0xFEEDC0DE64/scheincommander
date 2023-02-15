import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    Label {
        text: qsTr("Settings")
    }

    RowLayout {
        anchors.centerIn: parent

        Button {
            id: button0
            text: qsTr("Device\nTypes")

            Layout.preferredWidth: 100
            Layout.preferredHeight: 100

            onClicked: stackview.push(deviceTypesSettingsPage)

            Component {
                id: deviceTypesSettingsPage

                DeviceTypesSettingsPage {
                }
            }
        }

        Button {
            id: button1
            text: qsTr("Devices")

            Layout.preferredWidth: 100
            Layout.preferredHeight: 100

            onClicked: stackview.push(devicesSettingsPage)

            Component {
                id: devicesSettingsPage

                DevicesSettingsPage {
                }
            }
        }
    }
}
