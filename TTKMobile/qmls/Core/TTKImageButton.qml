import QtQuick 2.5

Rectangle {
    id: ttkImageButton

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
        onClicked: {
            ttkImageButton.pressed();
        }
    }
    color: ttkTheme.alphaLv0
}
