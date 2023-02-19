import QtQuick
import QtQuick.VirtualKeyboard

InputPanel {
    id: inputPanel

    states: State {
        name: "visible"
        when: inputPanel.active
        PropertyChanges {
            target: inputPanel
            y: window.height - inputPanel.height
        }
    }
    transitions: [
        Transition {
            from: "visible"
            to: ""
            reversible: false
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }
        },
        Transition {
            from: ""
            to: "visible"
            reversible: false
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 1000
                    easing.type: Easing.OutBounce
                }
            }
        }
    ]
}
