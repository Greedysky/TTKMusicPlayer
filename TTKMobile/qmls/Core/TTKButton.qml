import QtQuick 2.5

Rectangle {
    id: ttkButton
    property alias source: image.source

    signal pressed
    
    Image {
        id: image
        smooth: true
        anchors.fill: parent
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressed: {
            ttkButton.pressed();
        }
    }
    color: ttkTheme.alphaLv0
}
