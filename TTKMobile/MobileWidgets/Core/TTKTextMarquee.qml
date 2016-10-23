/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5

Rectangle {
    id: ttkTextMarquee
    clip: true

    property alias text: text.text
    property int fontSize: 24
    property int duration: 2000
    property alias color: text.color
    property alias font: text.font
    property bool marqueeEnable: false
    property alias xpos: text.x

    Text {
        id: text
        x: 0
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: ttkTextMarquee.fontSize
        clip: true

        SequentialAnimation on x {
            running: ttkTextMarquee.marqueeEnable
            loops: 1
            NumberAnimation {
                from: 0
                to: xpos
                duration: ttkTextMarquee.duration
            }
        }
    }
}
