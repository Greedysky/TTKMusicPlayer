import QtQuick 2.5

FocusScope {
    id: ttkLineInput

    property alias text: input.text
    property alias hint: hint.text
    property alias prefix: prefix.text
    property int fontSize: 15

    signal accepted

    Rectangle {
        anchors.fill: parent
        color: ttkTheme.alphaLv10
        radius: 1

        Text {
            id: hint
            anchors {
                fill: parent
                leftMargin: 14
            }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: ttkLineInput.fontSize
            color: ttkTheme.white
            opacity: input.length ? 0 : 1
        }

        Text {
            id: prefix
            anchors {
                left: parent.left
                leftMargin: 14;
                verticalCenter: parent.verticalCenter
            }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: ttkLineInput.fontSize
            color: ttkTheme.white
            opacity: !hint.opacity
        }

        TextInput {
            id: input
            focus: true
            anchors {
                left: prefix.right
                right: parent.right
                top: parent.top
                bottom: parent.bottom
            }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: ttkLineInput.fontSize
            color: ttkTheme.white
            onAccepted: {
                ttkLineInput.accepted();
            }
        }
    }
}
