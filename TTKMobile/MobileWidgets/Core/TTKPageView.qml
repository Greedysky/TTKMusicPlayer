/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5

ListView{
    id: ttkPageView
    width: parent.width
    height: parent.height
    model: pageModel
    clip: true

    default property alias content: pageModel.children

    VisualItemModel {
        id: pageModel
    }

    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    highlightRangeMode: ListView.StrictlyEnforceRange
    cacheBuffer: 3
    boundsBehavior: Flickable.StopAtBounds

    TTKListViewIndicator {
        target : ttkPageView
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        z: 2
    }
}
