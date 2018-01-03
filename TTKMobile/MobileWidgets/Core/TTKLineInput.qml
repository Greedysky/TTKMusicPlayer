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

FocusScope {
    id: ttkLineInput

    property alias text: input.text
    property alias validator: input.validator
    property alias hint: hint.text
    property alias prefix: prefix.text
    property int fontSize: 15

    signal accepted

    Rectangle {
        anchors.fill: parent
        color: ttkTheme.color_alpha_lv10
        radius: 1

        Text {
            id: hint
            anchors {
                fill: parent
                leftMargin: 14
            }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: ttkLineInput.fontSize
            color: ttkTheme.color_white
            opacity: input.length ? 0 : 1
        }

        Text {
            id: prefix
            anchors {
                left: parent.left
                leftMargin: 14
                verticalCenter: parent.verticalCenter
            }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: ttkLineInput.fontSize
            color: ttkTheme.color_white
            opacity: !hint.opacity
        }

        TextInput {
            id: input
            focus: true
            clip: true
            anchors {
                left: prefix.right
                right: parent.right
                rightMargin: 14
                top: parent.top
                bottom: parent.bottom
            }
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: ttkLineInput.fontSize
            color: ttkTheme.color_white
            onAccepted: {
                ttkLineInput.accepted();
            }
        }
    }
}
