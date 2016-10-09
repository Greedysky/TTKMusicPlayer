/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
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
    id: ttkMusicPlayerCenterSettingPage

    visible: false
    anchors.fill: parent
    color: ttkTheme.alphaLv12

    property alias text: musicSongTitle.text

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicPlayerCenterSettingPage.visible = false;
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height*0.7
        color: "#EEEEEE"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            Text {
                id: musicSongTitle
                anchors {
                    left: parent.left
                    leftMargin: dpHeight(10)
                }
            }

            Text {
                anchors {
                    left: parent.left
                    leftMargin: dpHeight(10)
                }
                text: qsTr("该歌曲信息为智能匹配结果")
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: "black"
            }

            ListView {
                anchors {
                    left: parent.left
                    leftMargin: dpHeight(30)
                    right: parent.right
                    rightMargin: dpHeight(30)
                }
                Layout.fillWidth: true
                Layout.preferredHeight: ttkMusicPlayerCenterSettingPage.height/10
                orientation: ListView.Horizontal
                spacing: dpHeight(30)

                delegate: Rectangle {
                    id: wrapper1
                    width: dpWidth(60)
                    height: dpHeight(60)

                    Rectangle {
                        id: imageArea1
                        width: parent.width
                        height: parent.height
                        color: ttkTheme.white
                        radius: 6

                        Image {
                            anchors.fill: parent
                            source: imgSource
                        }
                    }

                    Text {
                        anchors {
                            top: imageArea1.bottom
                            topMargin: dpHeight(20)
                        }
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        width: parent.width
                        text: title
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
                color: "black"
            }

            ListView {
                anchors {
                    left: parent.left
                    leftMargin: dpHeight(30)
                    right: parent.right
                    rightMargin: dpHeight(30)
                }
                Layout.fillWidth: true
                Layout.preferredHeight: ttkMusicPlayerCenterSettingPage.height/10
                orientation: ListView.Horizontal
                spacing: dpHeight(30)

                delegate: Rectangle {
                    id: wrapper2
                    width: dpWidth(60)
                    height: dpHeight(60)

                    Rectangle {
                        id: imageArea2
                        width: parent.width
                        height: parent.height
                        color: ttkTheme.white
                        radius: 6

                        Image {
                            anchors.fill: parent
                            source: imgSource
                        }
                    }

                    Text {
                        anchors {
                            top: imageArea2.bottom
                            topMargin: dpHeight(20)
                        }
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        width: parent.width
                        text: title
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
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: "black"
            }

            Slider {
                id: musicTimeSlider
                Layout.alignment: Qt.AlignCenter
                Layout.preferredWidth: ttkMusicPlayerCenterSettingPage.width - dpWidth(100)
                Layout.preferredHeight: dpHeight(30)
                height: 30
                minimumValue: 0
                value: 0

                function sliderGeometry() {
                    return (musicTimeSlider.value - musicTimeSlider.minimumValue) /
                           (musicTimeSlider.maximumValue - musicTimeSlider.minimumValue);
                }

                style: SliderStyle{
                    groove: Row {
                        Rectangle{
                            implicitWidth: musicTimeSlider.width*musicTimeSlider.sliderGeometry()
                            implicitHeight: dpHeight(3)
                            color: ttkTheme.topbar_background
                        }

                        Rectangle{
                            implicitWidth: musicTimeSlider.width*(1-musicTimeSlider.sliderGeometry())
                            implicitHeight: dpHeight(3)
                            color: "gray"
                        }
                    }

                    handle: Rectangle{
                        anchors.centerIn: parent;
                        color: ttkTheme.topbar_background
                        width: dpWidth(20)
                        height: dpHeight(20)
                        radius: dpWidth(10)
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: "black"
            }

            Text {
                Layout.alignment: Qt.AlignCenter
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                font.pixelSize: ttkMusicPlayerCenterSettingPage.height/25
                text: "取消"
            }
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicPlayerCenterSettingPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicPlayerCenterSettingPage.parent = docRoot;
    }
}
