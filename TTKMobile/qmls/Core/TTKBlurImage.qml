import QtQuick 2.5
import QtGraphicalEffects 1.0

Rectangle{
    id: ttkBlurImage
    clip: true

    property Item target

    FastBlur {
        id: fastBlur
        source: parent.target
        width: source.width
        height: source.height
        radius: 64
    }

    onXChanged: setBlurPosition();
    onYChanged: setBlurPosition();
    Component.onCompleted: setBlurPosition()

    function setBlurPosition() {
        fastBlur.x = target.x - x;
        fastBlur.y = target.y - y;
    }
}
