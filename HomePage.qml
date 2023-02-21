import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

import scheincommander

Item {
    id: homePage

    property bool needsRegler: true

    Button {
        anchors.top: parent.top
        anchors.right: parent.right
        width: 100
        height: 100

        text: qsTr('Settings')

        onClicked: stackview.push(settingsPage)

        Component {
            id: settingsPage

            SettingsPage {
            }
        }
    }

    Flow{
        anchors.fill: parent
        flow: Flow.TopToBottom

        Repeater {
            model: RegisterGroupsModel {
                controller: __controller
            }

            delegate: RowLayout {
                Label {
                    text: model.name
                }
                DmxSlider {
                    onValueChanged: __controller.setRegisterGroupSlider(model.id, value);
                }
            }
        }
    }
}
