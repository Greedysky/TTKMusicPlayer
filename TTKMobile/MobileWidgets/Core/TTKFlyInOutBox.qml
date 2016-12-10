/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Layouts 1.1

Rectangle {
    id: ttkFlyInOutBox
    width: parent.width
    height: ttkGlobal.dpHeight(ttkTheme.topbar_height)
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
                leftMargin: ttkGlobal.dpHeight(20)
            }
        }

        Text {
            id: textLabel
            color: ttkTheme.color_white
            font.pixelSize: ttkFlyInOutBox.height/3
            anchors {
                left: imageLabel.right
                leftMargin: ttkGlobal.dpHeight(10)
            }
        }
    }

    NumberAnimation {
        id: flyInAnimation
        target: ttkFlyInOutBox
        property: "y"
        from: -ttkFlyInOutBox.height
        to: 0
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
        from: 0
        to: -ttkFlyInOutBox.height
        duration: 400
        easing.type: Easing.InOutQuad
    }

    Timer {
        id: timer
        interval: 800
        repeat: false

        onTriggered: {
            flyOutAnimation.start();
        }
    }
}
