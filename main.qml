import QtQuick

LightControlWindow {
    Component.onCompleted: {
        if (__windowed)
            showNormal();
        else
            showFullScreen();
    }
}
