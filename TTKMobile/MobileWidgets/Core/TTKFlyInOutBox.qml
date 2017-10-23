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
