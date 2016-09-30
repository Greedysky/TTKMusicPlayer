import QtQuick 2.5
import QtQuick.Layouts 1.1

Rectangle {
    id: ttkFlyInOutBox
    width: parent.width
    height: dpHeight(ttkTheme.topbar_height)
    color: ttkTheme.topbar_background
    y: -height
    visible: true

    property alias text: textLabel.text
    function start() {
        flyInAnimation.start();
    }

    RowLayout {
        spacing: 2
        anchors.fill: parent

        Image {
            id: imageLabel
            Layout.preferredHeight: ttkFlyInOutBox.height/2
            Layout.preferredWidth: ttkFlyInOutBox.height/2
            source: "qrc:/image/bannertips_succes_icon"
            anchors {
                left: parent.left
                leftMargin: dpHeight(20)
            }
        }

        Text {
            id: textLabel
            color: ttkTheme.white
            font.pixelSize: ttkFlyInOutBox.height/3
            anchors {
                left: imageLabel.right
                leftMargin: dpHeight(10)
            }
        }
    }

    NumberAnimation {
        id: flyInAnimation
        target: ttkFlyInOutBox
        property: "y"
        from: -ttkFlyInOutBox.height
        to: ttkFlyInOutBox.height
        duration: 400
        easing.type: Easing.InOutQuad

        onStarted: {
            flyOutAnimation.stop();
            timer.stop();
        }
        onStopped: {
            timer.start();
        }
    }

    NumberAnimation {
        id: flyOutAnimation
        target: ttkFlyInOutBox
        property: "y"
        from: ttkFlyInOutBox.height
        to: -ttkFlyInOutBox.height
        duration: 400
        easing.type: Easing.InOutQuad
    }
    Timer{
        id: timer;
        interval: 800;
        repeat: false;

        onTriggered: {
            flyOutAnimation.start();
        }
    }
}
