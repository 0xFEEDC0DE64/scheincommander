import QtQuick
import QtQuick.Controls.Material

StackView {
    pushEnter: Transition {
        PropertyAnimation {
            property: "opacity"
            from: 0
            to:1
            duration: 200
        }
    }
    pushExit: Transition {
        PropertyAnimation {
            property: "opacity"
            from: 1
            to:0
            duration: 200
        }
    }
    popEnter: Transition {
        PropertyAnimation {
            property: "opacity"
            from: 0
            to:1
            duration: 200
        }
    }
    popExit: Transition {
        PropertyAnimation {
            property: "opacity"
            from: 1
            to:0
            duration: 200
        }
    }
}
