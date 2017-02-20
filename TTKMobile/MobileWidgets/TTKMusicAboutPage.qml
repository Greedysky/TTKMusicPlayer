/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Controls 1.4
import "Core"

Rectangle{
    id: ttkMusicAboutPage

    visible: false
    anchors.fill: parent
    color: ttkTheme.color_alpha_lv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicAboutPage.visible = false;
        }
    }

    Rectangle {
        id: mainRectangle
        color: ttkTheme.color_white
        radius: 10
        width: 0.7*parent.width
        height: 0.3*parent.height
        anchors.centerIn: parent

        Text {
            id: textArea
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height/11
            text: qsTr("TTK Music Player") + "\n\n" +
                  qsTr("Directed By Greedysky") +
                  ("\nCopyright© 2015-2017") +
                  ("\nMail:Greedysky@163.com");
        }

        TTKTextButton {
            anchors {
                bottom: parent.bottom
                left: parent.left
                bottomMargin: ttkGlobal.dpHeight(20)
                leftMargin: ttkGlobal.dpWidth(50)
            }
            textColor: ttkTheme.topbar_background
            text: qsTr("关于")
            onPressed: {
                Qt.openUrlExternally("https://github.com/Greedysky/TTKMusicplayer")
            }
        }

        TTKTextButton {
            anchors {
                bottom: parent.bottom
                right: parent.right
                bottomMargin: ttkGlobal.dpHeight(20)
                rightMargin: ttkGlobal.dpWidth(50)
            }
            textColor: ttkTheme.topbar_background
            text: qsTr("确定")
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicAboutPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicAboutPage.parent = docRoot;
    }
}
