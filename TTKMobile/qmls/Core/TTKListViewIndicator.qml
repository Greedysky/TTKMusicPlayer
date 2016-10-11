/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
            width: dpWidth(10)
            height: dpHeight(10)
            radius: dpWidth(6)
            color: target.currentIndex == index ? ttkTheme.white : ttkTheme.gray
            MouseArea {
                width: dpWidth(20)
                height: dpHeight(20)
                anchors.centerIn: parent
                onClicked: target.currentIndex = index
            }
        }
    }
}
