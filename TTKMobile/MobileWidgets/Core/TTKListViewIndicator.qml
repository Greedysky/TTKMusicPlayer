/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5

Row {
    property ListView target;
    spacing: 20

    Repeater {
        opacity: 0.8
        model: target.model.count
        Rectangle {
            width: ttkGlobal.dpWidth(10)
            height: ttkGlobal.dpHeight(10)
            radius: ttkGlobal.dpWidth(6)
            color: target.currentIndex == index ? ttkTheme.color_white : ttkTheme.color_gray
            MouseArea {
                width: ttkGlobal.dpWidth(20)
                height: ttkGlobal.dpHeight(20)
                anchors.centerIn: parent
                onClicked: target.currentIndex = index
            }
        }
    }
}
