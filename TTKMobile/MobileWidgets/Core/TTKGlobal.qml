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
    property int list_module_index;
}
