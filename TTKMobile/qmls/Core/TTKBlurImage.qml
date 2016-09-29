import QtQuick 2.5
import QtGraphicalEffects 1.0

Rectangle{
    id: ttkBlurImage
    clip: true

    property Item target

    FastBlur {
        id: blur
        source: parent.target
        width: source.width
        height: source.height
        radius: 64
    }

    onXChanged: setBlurPosition();
    onYChanged: setBlurPosition();
    Component.onCompleted: setBlurPosition()

    function setBlurPosition() {
        blur.x = target.x - x;
        blur.y = target.y - y;
    }
}
