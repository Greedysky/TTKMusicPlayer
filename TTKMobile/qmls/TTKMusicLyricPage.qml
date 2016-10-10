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
            color: "#00000000"
            Behavior on y {
                SmoothedAnimation {
                    duration: 300
                }
            }
        }

        delegate: Rectangle {
            id: lyricDelegateId
            width: parent.width
            height: 20
            color: "#00000000"

            Text {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                text: textLine
                color: parent.ListView.isCurrentItem ? "#00ff00" : "#4C4C4C"
                font.pixelSize: parent.ListView.isCurrentItem ? dpHeight(30) : dpHeight(26)
                font.bold: parent.ListView.isCurrentItem
                font.family: qsTr("楷体")
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.OpenHandCursor
            }
        }
    }
}
