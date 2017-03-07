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
    id: ttkMusicPlayerCenterSettingPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    property alias text: musicSongTitle.text
    property int muteVolume: TTK_PLAYER.volume()

    function removeItemFromList() {
        TTK_APP.removeMusicSongs();
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicPlayerCenterSettingPage.visible = false;
        }
    }

    Rectangle {
        width: parent.width
        height: parent.height*0.75
        color: "#F0F0F0"
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
                    leftMargin: ttkGlobal.dpHeight(10)
                }
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
                Layout.preferredHeight: ttkMusicPlayerCenterSettingPage.height/8
                orientation: ListView.Horizontal
                spacing: ttkGlobal.dpHeight(30)

                delegate: TTKImageRadiusTextButton {
                    source: imgSource
                    text: title
                    onClicked: {
                        switch(index) {
                            case 0: break;
                            case 1:
                                ttkMusicPlayerCenterSettingPage.visible = false;
                                ttkMusicSongDownloadPage.visible = true;
                                break;
                            case 2:
                                ttkMusicPlayerCenterSettingPage.visible = false;
                                ttkMusicSongSharedPage.visible = true;
                                break;
                            case 3:
                                ttkMusicDeletePage.visible = true;
                                ttkMusicPlayerCenterSettingPage.visible = false;
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
                Layout.preferredHeight: ttkMusicPlayerCenterSettingPage.height/8
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
                            case 7: break;
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
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: ttkTheme.color_alpha_lv10
            }

            Rectangle {
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true
                Layout.preferredHeight: ttkGlobal.dpHeight(30)
                color: ttkTheme.color_alpha_lv0

                RowLayout{
                    anchors.fill: parent

                    TTKImageButton {
                        id: musicVolumeLabel
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(30)
                        }
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: ttkGlobal.dpWidth(30)
                        Layout.preferredHeight: ttkGlobal.dpHeight(30)
                        source: "qrc:/image/playing_volumn_slide_icon"

                        onClicked: {
                            if(TTK_PLAYER.volume() !== 0) {
                                source = "qrc:/image/playing_volumn_slide_nosound_icon";
                                muteVolume = TTK_PLAYER.volume();
                                TTK_PLAYER.setMuted(true);
                                musicVolumeSlider.value = 0;
                            }else {
                                source = "qrc:/image/playing_volumn_slide_icon";
                                TTK_PLAYER.setMuted(false);
                                TTK_PLAYER.setVolume(muteVolume);
                                musicVolumeSlider.value = muteVolume;
                            }
                        }
                    }

                    Slider {
                        id: musicVolumeSlider
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: ttkMusicPlayerCenterSettingPage.width - ttkGlobal.dpWidth(100)
                        Layout.preferredHeight: ttkGlobal.dpHeight(30)
                        height: 30
                        minimumValue: 0
                        maximumValue: 100
                        value: TTK_PLAYER.volume()

                        function sliderGeometry() {
                            return (musicVolumeSlider.value - musicVolumeSlider.minimumValue) /
                                   (musicVolumeSlider.maximumValue - musicVolumeSlider.minimumValue);
                        }

                        style: SliderStyle{
                            groove: Row {
                                Rectangle{
                                    implicitWidth: musicVolumeSlider.width*musicVolumeSlider.sliderGeometry()
                                    implicitHeight: ttkGlobal.dpHeight(3)
                                    color: ttkTheme.topbar_background
                                }

                                Rectangle{
                                    implicitWidth: musicVolumeSlider.width*(1-musicVolumeSlider.sliderGeometry())
                                    implicitHeight: ttkGlobal.dpHeight(3)
                                    color: ttkTheme.color_gray
                                }
                            }

                            handle: Rectangle{
                                anchors.centerIn: parent
                                color: ttkTheme.topbar_background
                                width: ttkGlobal.dpWidth(20)
                                height: ttkGlobal.dpHeight(20)
                                radius: ttkGlobal.dpWidth(10)
                            }

                            panel: Rectangle{
                                anchors.fill: parent;
                                color: ttkTheme.color_alpha_lv0

                                Loader{
                                    id: grooveLoader
                                    anchors.centerIn: parent
                                    sourceComponent: groove
                                }

                                Loader{
                                    id: handleLoader
                                    anchors.verticalCenter: grooveLoader.verticalCenter;
                                    x: Math.min(grooveLoader.x + (control.value*grooveLoader.width)/(control.maximumValue - control.minimumValue) - item.width/2,
                                                grooveLoader.width)
                                    sourceComponent: handle
                                    onXChanged: {
                                        var value = 100/parent.width*x + 2.9;
                                        TTK_PLAYER.setVolume(value);
                                        TTK_PLAYER.volume() === 0 ? musicVolumeLabel.source = "qrc:/image/playing_volumn_slide_nosound_icon"
                                                                  : musicVolumeLabel.source = "qrc:/image/playing_volumn_slide_icon";
                                    }
                                }
                            }
                        }
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
                font.pixelSize: ttkMusicPlayerCenterSettingPage.height/25
                text: "取消"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMusicPlayerCenterSettingPage.visible = false;
                    }
                }
            }
        }
    }

    NumberAnimation {
        id: verticalYAnimation
        property: "y"
        target: ttkMusicPlayerCenterSettingPage
        from: ttkMusicPlayerCenterSettingPage.height
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
        var docRoot = ttkMusicPlayerCenterSettingPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicPlayerCenterSettingPage.parent = docRoot;
    }

    TTKMusicDeletePage {
        id: ttkMusicDeletePage
    }
}
