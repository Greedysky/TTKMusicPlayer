/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5

Rectangle {
    id: ttkImageButton

    property alias source: image.source

    signal clicked
    
    Image {
        id: image
        smooth: true
        anchors.fill: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            ttkImageButton.clicked();
        }
    }

    color: ttkTheme.color_alpha_lv0
}
