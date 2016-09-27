import QtQuick 2.5
import QtGraphicalEffects 1.0

Rectangle {
    id: ttkRadiusImage
    property alias foreground: fImage.source
    property alias background: bImage.source

    Image {
        id: fImage
        anchors.fill: parent
        smooth: true
        visible: false
    }

    Image {
        id: bImage
        anchors.fill: parent
        smooth: true
        visible: false
    }

    OpacityMask {
        anchors.fill: fImage
        source: fImage
        maskSource: bImage
    }
}
