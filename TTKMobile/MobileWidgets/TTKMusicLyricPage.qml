/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.4

Rectangle {
    id: ttkMusicLyricPage

    property alias currentIndex: lyricView.currentIndex

    ListView {
        id: lyricView
        anchors.fill: parent
        highlightRangeMode: ListView.StrictlyEnforceRange
        clip: true
        spacing: 15

        model: TTK_LRC

        highlight: Rectangle {
            color: ttkTheme.color_alpha_lv0
            Behavior on y {
                SmoothedAnimation {
                    duration: 300
                }
            }
        }

        delegate: Rectangle {
            width: parent.width
            height: ttkGlobal.dpHeight(35)*(Math.ceil(TTK_UTILS.stringFontWidth(textArea.font, textLine)*1.0/parent.width))
            color: ttkTheme.color_alpha_lv0

            Text {
                id: textArea
                anchors.centerIn: parent
                width: lyricView.width
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                color: parent.ListView.isCurrentItem ? ttkTheme.color_green : ttkTheme.color_alpha_lv6
                font.pixelSize: parent.ListView.isCurrentItem ? ttkGlobal.dpHeight(26) : ttkGlobal.dpHeight(22)
                font.bold: parent.ListView.isCurrentItem
                font.family: qsTr("楷体")
                text: textLine
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.OpenHandCursor
            }
        }
    }
}
