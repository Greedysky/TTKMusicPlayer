/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5

Rectangle {
    id: ttkImageRadiusTextButton
    width: ttkGlobal.dpWidth(60)
    height: ttkGlobal.dpHeight(60)
    radius: 10

    property alias source: image.source
    property alias text: text.text
    signal clicked

    Rectangle {
        id: imageArea
        width: parent.width
        height: parent.height
        color: ttkTheme.color_white
        radius: parent.radius

        Image {
            id: image
            anchors.fill: parent
        }
    }

    Text {
        id: text
        anchors {
            top: imageArea.bottom
            topMargin: ttkGlobal.dpHeight(20)
        }
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignHCenter
        width: parent.width
        color: ttkTheme.color_gray
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkImageRadiusTextButton.clicked();
        }
    }
}
