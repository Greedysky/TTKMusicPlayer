/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtGraphicalEffects 1.0

Rectangle {
    id: ttkRadiusImage
    property alias foreground: fImage.source
    property alias background: bImage.source

    Image {
        id: fImage
        anchors.fill: parent
        smooth: true
        visible: false
    }

    Image {
        id: bImage
        anchors.fill: parent
        smooth: true
        visible: false
    }

    OpacityMask {
        anchors.fill: fImage
        source: fImage
        maskSource: bImage
    }
}
