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
import QtQuick.Layouts 1.1

Rectangle {
    id: ttkMainFunctionItem
    width: parent.width/3
    height: (parent.height-10)/2

    property alias source: image.source
    property alias mainTitle: mainTitle.text
    property alias subTitle: subTitle.text
    property int mainTitleSize: height/8

    signal clicked

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMainFunctionItem.clicked();
        }
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        Rectangle {
            Layout.preferredWidth: ttkGlobal.dpWidth(60)
            Layout.preferredHeight: ttkGlobal.dpHeight(60)
            Layout.alignment: Qt.AlignCenter
            color: ttkTheme.color_alpha_lv0

            Image {
                id: image
                anchors.fill: parent
            }
        }

        Text {
            id: mainTitle
            Layout.alignment: Qt.AlignCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: mainTitleSize
        }

        Text {
            id: subTitle
            Layout.alignment: Qt.AlignCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: ttkTheme.color_gray
        }
    }
}
