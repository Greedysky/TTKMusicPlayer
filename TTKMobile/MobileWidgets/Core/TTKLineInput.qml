/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
