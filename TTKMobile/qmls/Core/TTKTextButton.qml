import QtQuick 2.5

Rectangle {
    id: ttkTextButton

    property alias text: textArea.text
    property alias textColor: textArea.color
    property alias textSize: textArea.font.pixelSize

    signal pressed
    
    Text {
        id: textArea
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            ttkTextButton.pressed();
        }
    }
    color: ttkTheme.alphaLv0
}
