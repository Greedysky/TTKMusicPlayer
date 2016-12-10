/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5

Rectangle {
    id: ttkTextPair

    property alias textFirst: firstTextArea.text
    property alias textSecond: secondTextArea.text
    property alias colorFirst: firstTextArea.text
    property alias colorSecond: secondTextArea.text
    property alias textSizeFirst: firstTextArea.text
    property alias textSizeSecond: secondTextArea.text

    Text {
        id: firstTextArea
        color: ttkTheme.color_gray
    }

    Text {
        anchors {
            top: firstTextArea.bottom
            topMargin: ttkGlobal.dpWidth(20)
        }
        id: secondTextArea
        color: ttkTheme.color_black
    }
}
