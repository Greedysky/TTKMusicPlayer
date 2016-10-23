/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Layouts 1.1

Rectangle {
    id: ttkMainFunctionItem
    width: parent.width/3
    height: (parent.height-10)/2

    property alias source: image.source
    property alias mainTitle: mainTitle.text
    property alias subTitle: subTitle.text

    signal pressed

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMainFunctionItem.pressed();
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
            font.pixelSize: parent.height/8
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
