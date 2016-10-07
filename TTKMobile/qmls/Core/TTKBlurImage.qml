/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtGraphicalEffects 1.0

Rectangle{
    id: ttkBlurImage
    clip: true

    property Item target

    FastBlur {
        id: fastBlur
        source: parent.target
        width: source.width
        height: source.height
        radius: 64
    }

    onXChanged: setBlurPosition();
    onYChanged: setBlurPosition();
    Component.onCompleted: setBlurPosition()

    function setBlurPosition() {
        fastBlur.x = target.x - x;
        fastBlur.y = target.y - y;
    }
}
