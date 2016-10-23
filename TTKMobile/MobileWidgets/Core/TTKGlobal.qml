/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Window 2.0

QtObject {
    ////////////////////////////////////////////
    property int window_width: isAndroid() ? Screen.desktopAvailableWidth : 320
    property int window_height: isAndroid() ? Screen.desktopAvailableHeight : 480

    property real pixel_density: 4.46
    property real multiplier_height: window_height/480
    property real multiplier_width: window_width/320

    function isAndroid() {
        return Qt.platform.os === "android";
    }
    function dpHeight(numbers) {
        return Math.round(numbers*((pixel_density*25.4)/160)*multiplier_height);
    }
    function dpWidth(numbers) {
        return Math.round(numbers*((pixel_density*25.4)/160)*multiplier_height);
    }
    ////////////////////////////////////////////
    property string video_url;
}
