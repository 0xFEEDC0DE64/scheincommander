import QtQuick
import QtQuick.Controls

SpinBox {
    id: spinbox

    property int decimals: 2
    property real factor: 100

    property real realFrom: from / factor
    property real realTo: to / factor
    property real realValue: value / factor

    from: realFrom * factor
    to: realTo * factor
    value: realValue * factor

    stepSize: factor

    signal realValueModified

    onValueModified: {
        realValue = Qt.binding(function(){ return value / factor; });
        realValueModified();
    }

    validator: DoubleValidator {
        bottom: Math.min(spinbox.from, spinbox.to)
        top:  Math.max(spinbox.from, spinbox.to)
    }

    textFromValue: function(value, locale) {
        return Number(value / 100).toLocaleString(locale, 'f', spinbox.decimals)
    }

    valueFromText: function(text, locale) {
        return Number.fromLocaleString(locale, text) * 100
    }
}
