/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5

Rectangle {
    id: ttkTextButton
    color: ttkTheme.color_alpha_lv0

    property alias text: textArea.text
    property alias textColor: textArea.color
    property alias textSize: textArea.font.pixelSize

    signal clicked
    
    Text {
        id: textArea
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            ttkTextButton.clicked();
        }
    }
}
