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
import "Core"

Rectangle {
    id: ttkMusicBar
    anchors.right: ttkMainWindow.right
    anchors.bottom: ttkMainWindow.bottom
    width: ttkMainWindow.width
    height: ttkGlobal.dpHeight(70)
    border {
        color: ttkTheme.color_alpha_lv9
        width: 1
    }
    color: ttkTheme.color_white
    Component.onCompleted: {
        nameTitle = TTK_APP.empty() ? qsTr("天天酷音") : TTK_APP.mediaName();
        artistTitle = TTK_APP.empty() ? qsTr("天天酷音") : TTK_APP.mediaArtist();
    }

    property alias nameTitle: musicNameTitle.text
    property alias artistTitle: musicArtistTitle.text

    function playStateChanged() {
        if(TTK_PLAYER.state() === 1) {
            barPlayButton.source = "qrc:/image/landscape_player_btn_pause_normal";
            musicBarImageAnimation.resume();
            musicBarImageAnimation.start();
        }else {
            barPlayButton.source = "qrc:/image/landscape_player_btn_play_normal";
            musicBarImageAnimation.pause();
        }
    }

    Connections {
        target: TTK_PLAYER
        onDurationChanged: {
            TTK_APP.musicLoadCurrentSongLrc();
        }
    }

    Connections {
        target: TTK_APP
        onCurrentIndexChanged: {
            musicBarImage.foreground = TTK_APP.artistImagePath().length === 0 ? "qrc:/image/landscape_check_album_normal"
                                                                              : TTK_APP.artistImagePath();
            TTK_PLAYER.play();
            ttkMusicBar.nameTitle = TTK_APP.mediaName();
            ttkMusicBar.artistTitle = TTK_APP.mediaArtist();
            ttkMusicBar.playStateChanged();
        }
        onUpdateCurrentArtist: {
            musicBarImage.foreground = TTK_APP.artistImagePath().length === 0 ? "qrc:/image/landscape_check_album_normal"
                                                                              : TTK_APP.artistImagePath();
        }
        onEmptyPlayerCenter: {
            nameTitle = qsTr("天天酷音");
            artistTitle = qsTr("天天酷音");
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if(!TTK_APP.empty()) {
                ttkOutStackView.push("qrc:/MobileWidgets/TTKMusicPlayerCenterPage.qml");
            }
        }
    }

    TTKRadiusImage {
        id: musicBarImage
        width: parent.height
        height: parent.height
        anchors {
            left: parent.left
            top: parent.top
            leftMargin: ttkGlobal.dpHeight(10)
        }
        color: ttkTheme.color_alpha_lv0
        foreground: TTK_APP.artistImagePath().empty ? "qrc:/image/landscape_check_album_normal"
                                                    : TTK_APP.artistImagePath()
        background: "qrc:/image/radius_mask"

        RotationAnimation {
            id: musicBarImageAnimation
            target: musicBarImage
            property: "rotation"
            from: 0
            to: 360
            direction: RotationAnimation.Clockwise
            duration: 30000
            loops: Animation.Infinite
        }
    }

    Text {
        id: musicNameTitle
        width: ttkMusicBar.width - barPlayButton.width - barPlayListButton.width - musicBarImage.width - ttkGlobal.dpHeight(30)
        anchors {
            left: musicBarImage.right
            top: parent.top
            leftMargin: ttkGlobal.dpHeight(5)
            topMargin: ttkGlobal.dpHeight(10)
        }
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        font.pixelSize: parent.height/4
    }

    Text {
        id: musicArtistTitle
        width: musicNameTitle.width
        anchors {
            left: musicBarImage.right
            top: musicNameTitle.bottom
            leftMargin: ttkGlobal.dpHeight(5)
            topMargin: ttkGlobal.dpHeight(10)
        }
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: ttkTheme.color_gray
    }

    TTKImageButton {
        id: barPlayListButton
        source: "qrc:/image/minibar_btn_playlist_normal"
        anchors {
            right: parent.right
            top: parent.top
            topMargin: (parent.height - height)/2
            rightMargin: ttkGlobal.dpHeight(10)
        }
        width: height
        height: ttkGlobal.dpHeight(60)
    }

    TTKImageButton {
        id: barPlayButton
        source: "qrc:/image/landscape_player_btn_play_normal"
        anchors {
            right: barPlayListButton.left
            top: parent.top
            topMargin: (parent.height - height)/2
            rightMargin: ttkGlobal.dpHeight(10)
        }
        width: height*1.2
        height: ttkGlobal.dpHeight(50)
        onPressed: {
            if(TTK_APP.empty()) {
                return;
            }
            if(TTK_PLAYER.state() === 1) {
                barPlayButton.source = "qrc:/image/landscape_player_btn_play_normal";
                TTK_PLAYER.pause();
                musicBarImageAnimation.pause();
            }else {
                barPlayButton.source = "qrc:/image/landscape_player_btn_pause_normal";
                TTK_PLAYER.play();
                musicBarImageAnimation.resume();
                musicBarImageAnimation.start();
            }
        }
    }
}
