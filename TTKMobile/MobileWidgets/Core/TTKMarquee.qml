import QtQuick 2.5

Rectangle {
    id: ttkMarquee
    clip: true
    width: parent.width
    height: marqueeText.height + padding
    
    property string text
    property int padding: 10
    property int speed: 2
    property int fontSize: 20
    property int interval: 100
    property color textColor: ttkTheme.color_black

    Text {
        id: marqueeText
        horizontalAlignment: Qt.AlignHCenter
        anchors.verticalCenter: parent.verticalCenter;
        font.pixelSize: fontSize
        color: textColor
        text: parent.text

        onTextChanged: {
            var delta = parent.width - TTK_UTILS.stringFontWidth(font, text);
            if(delta < 0) {
                timer.start();
                marqueeText.x = 0;
            }else {
                timer.stop();
                marqueeText.x = delta/2;
            }
        }
    }
    
    Timer {
        id: timer
        interval: parent.interval
        onTriggered: moveMarquee()
        repeat: true
    }

    function moveMarquee()
    {
        if(marqueeText.x + marqueeText.width < 0)
        {
            marqueeText.x = marqueeText.parent.width;
        }
        marqueeText.x -= speed;
    }
}
