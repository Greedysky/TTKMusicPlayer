/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import "Core"

Rectangle{
    id: ttkMusicSongSettingPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    property string songName
    property string singerName;
    property string filePath;

    TTKMusicSongInfoPage {
        id: ttkMusicSongInfoPage
    }

    TTKMusicSongSharedPage {
        id: ttkMusicSongSharedPage
    }

    TTKMusicSongDownloadPage {
        id: ttkMusicSongDownloadPage
        text: singerName + "-" + songName;
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicSongSettingPage.visible = false;
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height*0.6
        color: "#F0F0F0"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            Text {
                anchors {
                    left: parent.left
                    leftMargin: ttkGlobal.dpHeight(10)
                }
                text: songName
            }

            Text {
                anchors {
                    left: parent.left
                    leftMargin: ttkGlobal.dpHeight(10)
                }
                text: qsTr("该歌曲信息为智能匹配结果")
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: ttkTheme.color_alpha_lv10
            }

            ListView {
                anchors {
                    left: parent.left
                    leftMargin: ttkGlobal.dpHeight(30)
                    right: parent.right
                    rightMargin: ttkGlobal.dpHeight(30)
                }
                Layout.fillWidth: true
                Layout.preferredHeight: ttkMusicSongSettingPage.height/8
                orientation: ListView.Horizontal
                spacing: ttkGlobal.dpHeight(30)

                delegate: TTKImageRadiusTextButton {
                    source: imgSource
                    text: title
                    onClicked: {
                        switch(index) {
                            case 0: break;
                            case 1:
                                ttkMusicSongSettingPage.visible = false;
                                ttkMusicSongDownloadPage.visible = true;
                                break;
                            case 2:
                                ttkMusicSongSettingPage.visible = false;
                                ttkMusicSongSharedPage.visible = true;
                                break;
                            case 3:
                                ttkMusicDeletePage.visible = true;
                                ttkMusicSongSettingPage.visible = false;
                                break;
                            case 4: break;
                            case 5: break;
                            case 6: break;
                        }
                    }
                }

                model: ListModel {
                    ListElement {
                        imgSource: "qrc:/image/action_add_to_list_normal"
                        title: qsTr("加到歌单")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_download_normal"
                        title: qsTr("下载")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_share_normal"
                        title: qsTr("分享")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_delete_normal"
                        title: qsTr("删除")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_comment_normal"
                        title: qsTr("评论")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_sing_this_song_normal"
                        title: qsTr("唱这首歌")
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: ttkTheme.color_alpha_lv10
            }

            ListView {
                anchors {
                    left: parent.left
                    leftMargin: ttkGlobal.dpHeight(30)
                    right: parent.right
                    rightMargin: ttkGlobal.dpHeight(30)
                }
                Layout.fillWidth: true
                Layout.preferredHeight: ttkMusicSongSettingPage.height/8
                orientation: ListView.Horizontal
                spacing: ttkGlobal.dpHeight(30)

                delegate: TTKImageRadiusTextButton {
                    source: imgSource
                    text: title
                    onClicked: {
                        switch(index) {
                            case 0: break;
                            case 1: break;
                            case 2: break;
                            case 3: break;
                            case 4: break;
                            case 5: break;
                            case 6: break;
                            case 7:
                                ttkMusicSongSettingPage.visible = false;
                                ttkMusicSongInfoPage.visible = true;
                                break;
                        }
                    }
                }

                model: ListModel {
                    ListElement {
                        imgSource: "qrc:/image/action_check_singer_normal"
                        title: qsTr("查看歌手")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_check_album_normal"
                        title: qsTr("查看专辑")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_create_radio_normal"
                        title: qsTr("相似歌曲")
                    }
                    ListElement {
                        imgSource: "qrc:/image/actionsheet_pplayer_normal"
                        title: qsTr("百变播放器")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_lyric_poster_normal"
                        title: qsTr("歌词海报")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_ring_normal"
                        title: qsTr("设置铃声")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_icon_qzone"
                        title: qsTr("背景音乐")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_icon_info"
                        title: qsTr("歌曲信息")
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: ttkTheme.color_alpha_lv10
            }

            Text {
                Layout.alignment: Qt.AlignCenter
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                font.pixelSize: ttkMusicSongSettingPage.height/25
                text: "取消"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMusicSongSettingPage.visible = false;
                    }
                }
            }
        }
    }

    NumberAnimation {
        id: verticalYAnimation
        property: "y"
        target: ttkMusicSongSettingPage
        from: ttkMusicSongSettingPage.height
        to: 0
        duration: 200
    }

    onXChanged: {
        ttkMusicSongInfoPage.visible = false;
        ttkMusicSongSharedPage.visible = false;
        ttkMusicSongDownloadPage.visible = false;
    }

    onVisibleChanged: {
        if(visible === true) {
            verticalYAnimation.start();
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicSongSettingPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicSongSettingPage.parent = docRoot;
    }

    TTKMusicDeletePage {
        id: ttkMusicDeletePage
    }
}
