/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
