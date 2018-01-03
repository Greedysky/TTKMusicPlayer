/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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
    id: ttkImageRadiusTextButton
    width: ttkGlobal.dpWidth(60)
    height: ttkGlobal.dpHeight(60)
    radius: 10

    property alias source: image.source
    property alias text: text.text
    signal clicked

    Rectangle {
        id: imageArea
        width: parent.width
        height: parent.height
        color: ttkTheme.color_white
        radius: parent.radius

        Image {
            id: image
            anchors.fill: parent
        }
    }

    Text {
        id: text
        anchors {
            top: imageArea.bottom
            topMargin: ttkGlobal.dpHeight(20)
        }
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
        width: parent.width
        color: ttkTheme.color_gray
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkImageRadiusTextButton.clicked();
        }
    }
}
