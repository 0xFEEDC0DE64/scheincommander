import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Material

GridLayout {
    columns: 2

    property vector3d value
    onValueChanged: {
        xBox.realValue = value.x
        yBox.realValue = value.y
        zBox.realValue = value.z
    }

    signal valueModified

    function updateValue() {
        const newValue = Qt.vector3d(xBox.realValue, yBox.realValue, zBox.realValue);
        console.log(newValue);
        if (newValue === value)
            return;
        value = newValue;
        valueModified();
    }

    Label {
        text: qsTr('x:')
    }

    DoubleSpinBox {
        id: xBox
        editable: true
        realFrom: -1000
        realTo: 1000
        //stepSize: 0.01
        onRealValueModified: updateValue()
    }

    Label {
        text: qsTr('y:')
    }

    DoubleSpinBox {
        id: yBox
        editable: true
        realFrom: -1000
        realTo: 1000
        //stepSize: 0.01
        onRealValueModified: updateValue()
    }

    Label {
        text: qsTr('z:')
    }

    DoubleSpinBox {
        id: zBox
        editable: true
        realFrom: -1000
        realTo: 1000
        //stepSize: 0.01
        onRealValueModified: updateValue()
    }
}
