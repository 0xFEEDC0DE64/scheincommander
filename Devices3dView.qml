import QtQuick
import QtDataVisualization
import QtQuick.Controls

Item {
    property int selectedAxisLabel: -1
    property real dragSpeedModifier: 25.0
    property int currentMouseX: -1
    property int currentMouseY: -1
    property int previousMouseX: -1
    property int previousMouseY: -1
    property alias model: itemModelScatterDataProxy.itemModel

    property alias selectedItem: scatterSeries.selectedItem

//    ThemeColor {
//        id: dynamicColor
//        ColorAnimation on color {
//            from: "red"
//            to: "yellow"
//            duration: 2000
//            loops: Animation.Infinite
//        }
//    }

    Theme3D {
        id: dynamicColorTheme
        type: Theme3D.ThemeEbony
//        baseColors: [dynamicColor]
        font.pointSize: 50
        labelBorderEnabled: true
        labelBackgroundColor: "gold"
        labelTextColor: "black"
    }

    Theme3D {
        id: isabelleTheme
        type: Theme3D.ThemeIsabelle
        font.pointSize: 50
        labelBorderEnabled: true
        labelBackgroundColor: "gold"
        labelTextColor: "black"
    }

    Item {
        id: dataView
        anchors.bottom: parent.bottom
        width: parent.width
        height: parent.height

        //! [0]
        Scatter3D {
            id: scatterGraph
            inputHandler: null
            width: dataView.width
            height: dataView.height
            theme: isabelleTheme
            shadowQuality: AbstractGraph3D.ShadowQualityLow
            scene.activeCamera.yRotation: 45.0
            scene.activeCamera.xRotation: 45.0
            scene.activeCamera.zoomLevel: 75.0
            axisX.min: -10
            axisX.max: 10
            axisY.min: -10
            axisY.max: 10
            axisZ.min: -10
            axisZ.max: 10

            Scatter3DSeries {
                id: scatterSeries
                itemLabelFormat: "X:@xLabel Y:@yLabel Z:@zLabel"
                mesh: Abstract3DSeries.MeshCube

                ItemModelScatterDataProxy {
                    id: itemModelScatterDataProxy
                    xPosRole: "positionX"
                    yPosRole: "positionY"
                    zPosRole: "positionZ"
                    //rotationRole: "rotation"
                }
            }
            onSelectedElementChanged: {
                switch (selectedElement) {
                case Scatter3D.ElementSeries:
//                    console.log('series');
                    break;
                case Scatter3D.ElementAxisXLabel:
                case Scatter3D.ElementAxisYLabel:
                case Scatter3D.ElementAxisZLabel:
//                    console.log('axis');
                    if (selectedElement >= AbstractGraph3D.ElementAxisXLabel
                            && selectedElement <= AbstractGraph3D.ElementAxisZLabel)
                        selectedAxisLabel = selectedElement
                    else
                        selectedAxisLabel = -1
                    break;
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton

            onPositionChanged: (mouse)=> {
                currentMouseX = mouse.x;
                currentMouseY = mouse.y;

                if (pressed && selectedAxisLabel != -1)
                    dragAxis();

                previousMouseX = currentMouseX;
                previousMouseY = currentMouseY;
            }

            onPressed: (mouse)=> {
                scatterGraph.scene.selectionQueryPosition = Qt.point(mouse.x, mouse.y);
            }

            onReleased: {
                // We need to clear mouse positions and selected axis, because touch devices cannot
                // track position all the time
                selectedAxisLabel = -1
                currentMouseX = -1
                currentMouseY = -1
                previousMouseX = -1
                previousMouseY = -1
            }
        }
    }

    function dragAxis() {
        // Do nothing if previous mouse position is uninitialized
        if (previousMouseX === -1)
            return

        // Directional drag multipliers based on rotation. Camera is locked to 45 degrees, so we
        // can use one precalculated value instead of calculating xx, xy, zx and zy individually
        var cameraMultiplier = 0.70710678

        // Calculate the mouse move amount
        var moveX = currentMouseX - previousMouseX
        var moveY = currentMouseY - previousMouseY

        // Adjust axes
        switch (selectedAxisLabel) {
        case AbstractGraph3D.ElementAxisXLabel:
            var distance = ((moveX - moveY) * cameraMultiplier) / dragSpeedModifier
            // Check if we need to change min or max first to avoid invalid ranges
            if (distance > 0) {
                scatterGraph.axisX.min -= distance
                scatterGraph.axisX.max -= distance
            } else {
                scatterGraph.axisX.max -= distance
                scatterGraph.axisX.min -= distance
            }
            break
        case AbstractGraph3D.ElementAxisYLabel:
            distance = moveY / dragSpeedModifier
            // Check if we need to change min or max first to avoid invalid ranges
            if (distance > 0) {
                scatterGraph.axisY.max += distance
                scatterGraph.axisY.min += distance
            } else {
                scatterGraph.axisY.min += distance
                scatterGraph.axisY.max += distance
            }
            break
        case AbstractGraph3D.ElementAxisZLabel:
            distance = ((moveX + moveY) * cameraMultiplier) / dragSpeedModifier
            // Check if we need to change min or max first to avoid invalid ranges
            if (distance > 0) {
                scatterGraph.axisZ.max += distance
                scatterGraph.axisZ.min += distance
            } else {
                scatterGraph.axisZ.min += distance
                scatterGraph.axisZ.max += distance
            }
            break
        }
    }

    Button {
        id: rangeToggle
        width: parent.width / 2 // We're adding 3 buttons and want to divide them equally
        text: "Use Preset Range"
        anchors.left: parent.left
        property bool autoRange: true
        onClicked: {
            if (autoRange) {
                text = "Use Automatic Range"
                scatterGraph.axisX.min = -10
                scatterGraph.axisX.max = 10
                scatterGraph.axisY.min = -10
                scatterGraph.axisY.max = 10
                scatterGraph.axisZ.min = -10
                scatterGraph.axisZ.max = 10
                autoRange = false
                dragSpeedModifier = 25.0
            } else {
                text = "Use Preset Range"
                autoRange = true
                dragSpeedModifier = 25.0
            }
            scatterGraph.axisX.autoAdjustRange = autoRange
            scatterGraph.axisY.autoAdjustRange = autoRange
            scatterGraph.axisZ.autoAdjustRange = autoRange
        }
    }

    Button {
        id: orthoToggle
        width: parent.width / 2
        text: "Display Orthographic"
        anchors.left: rangeToggle.right
        onClicked: {
            if (scatterGraph.orthoProjection) {
                text = "Display Orthographic";
                scatterGraph.orthoProjection = false
                // Orthographic projection disables shadows, so we need to switch them back on
                scatterGraph.shadowQuality = AbstractGraph3D.ShadowQualityLow
            } else {
                text = "Display Perspective";
                scatterGraph.orthoProjection = true
            }
        }
    }
}
