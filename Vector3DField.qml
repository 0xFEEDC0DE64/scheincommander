import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

RowLayout {
    property vector3d value
    onValueChanged: {
        xBox.value = value.x
        yBox.value = value.y
        zBox.value = value.z
    }

    signal valueModified

    function updateValue() {
        const newValue = Qt.vector3d(xBox.value, yBox.value, zBox.value);
        console.log(newValue);
        if (newValue === value)
            return;
        value = newValue;
        valueModified();
    }

    Label {
        text: qsTr('x:')
    }

    SpinBox {
        id: xBox
        onValueModified: updateValue()
    }

    Label {
        text: qsTr('y:')
    }

    SpinBox {
        id: yBox
        onValueModified: updateValue()
    }

    Label {
        text: qsTr('z:')
    }

    SpinBox {
        id: zBox
        onValueModified: updateValue()
    }
}
