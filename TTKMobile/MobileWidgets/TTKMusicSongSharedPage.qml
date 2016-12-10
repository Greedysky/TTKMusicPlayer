/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Layouts 1.1
import "Core"

Rectangle {
    id: ttkMusicSongSharedPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicSongSharedPage.visible = false;
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height*0.5
        color: "#F0F0F0"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            Rectangle {
                Layout.fillWidth: true
                height: parent.height*0.41

                RowLayout {
                    spacing: 0
                    anchors.fill: parent

                    TTKMainFunctionItem {
                        width:  parent.height*0.9
                        height:  parent.height*0.9
                        source: "qrc:/image/share_item_qq"
                        mainTitle: qsTr("QQ好友")
                    }

                    TTKMainFunctionItem {
                        width:  parent.height*0.9
                        height:  parent.height*0.9
                        source: "qrc:/image/share_item_qzone"
                        mainTitle: qsTr("QQ空间")
                    }

                    TTKMainFunctionItem {
                        width:  parent.height*0.9
                        height:  parent.height*0.9
                        source: "qrc:/image/share_item_wx_friend"
                        mainTitle: qsTr("微信好友")
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                height: parent.height*0.41

                RowLayout {
                    spacing: 0
                    anchors.fill: parent

                    TTKMainFunctionItem {
                        width:  parent.height*0.9
                        height:  parent.height*0.9
                        source: "qrc:/image/share_item_wx_timeline"
                        mainTitle: qsTr("朋友圈")
                    }

                    TTKMainFunctionItem {
                        width:  parent.height*0.9
                        height:  parent.height*0.9
                        source: "qrc:/image/share_item_sina_weibo"
                        mainTitle: qsTr("新浪微博")
                    }

                    TTKMainFunctionItem {
                        width:  parent.height*0.9
                        height:  parent.height*0.9
                        source: "qrc:/image/share_item_tencent_weibo"
                        mainTitle: qsTr("腾讯微博")
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: ttkTheme.color_alpha_lv10
            }

            TTKTextButton {
                width: parent.width
                height: ttkGlobal.dpHeight(60)
                textColor: ttkTheme.color_black
                text: qsTr("取消")
                textSize: ttkMusicSongSharedPage.height/25
                onPressed: {
                    ttkMusicSongSharedPage.visible = false;
                }
            }
        }
    }

    NumberAnimation {
        id: verticalYAnimation
        property: "y"
        target: ttkMusicSongSharedPage
        from: ttkMusicSongSharedPage.height
        to: 0
        duration: 200
    }

    onVisibleChanged: {
        if(visible === true) {
            verticalYAnimation.start();
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicSongSharedPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicSongSharedPage.parent = docRoot;
    }
}
