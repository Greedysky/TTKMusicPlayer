/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import "Core"

Item {
    id: ttkMusicPlayerCenter
    width: parent.width
    height: parent.height

    property int playMode: showPlayModeText(TTK_APP.playbackMode())

    function showPlayModeText(value) {
        switch(value) {
            case 2:
                ttkFlyInOutBox.text = qsTr("已切换至随机播放")
                playerMode.source = "qrc:/image/player_btn_random_normal";
                break;
            case 3:
                ttkFlyInOutBox.text = qsTr("已切换至顺序播放")
                playerMode.source = "qrc:/image/player_btn_repeat_normal";
                break;
            case 4:
                ttkFlyInOutBox.text = qsTr("已切换至单曲播放")
                playerMode.source = "qrc:/image/player_btn_repeatone_normal";
                break;
        }
        return value;
    }

    function changedPlayState() {
        playerPlayButton.source = "qrc:/image/player_btn_pause_normal";
        artistImageAnimation.resume();
        artistImageAnimation.start();
    }

    function playStateChanged() {
        if(TTK_PLAYER.state() === 1) {
            changedPlayState();
        }else {
            playerPlayButton.source = "qrc:/image/player_btn_play_normal";
            artistImageAnimation.pause();
        }
    }

    Connections {
        target: TTK_PLAYER
        onDurationChanged: {
            musicTimeSlider.maximumValue = duration;
            musicTimeSlider.value = 0;
            durationLabel.text = TTK_UTILS.normalizeTime(duration, "mm:ss");
        }
        onPositionChanged: {
            TTK_LRC.findText(position);
            musicTimeSlider.maximumValue = TTK_PLAYER.duration();
            musicTimeSlider.value = position;
            durationLabel.text = TTK_UTILS.normalizeTime(TTK_PLAYER.duration(), "mm:ss");
            positionLabel.text = TTK_UTILS.normalizeTime(position, "mm:ss");
        }
        onMusicEnhanceChanged: {
            dtsEffectButton.source = TTK_PLAYER.getMusicEnhanced() === 0 ? "qrc:/image/player_btn_dts_off"
                                                                         : "qrc:/image/player_btn_dts_on"
        }
    }

    Connections {
        target: TTK_LRC
        onCurrentIndexChanged: {
            musicLrcShow.currentIndex = index;
        }
    }

    Connections {
        target: TTK_APP
        onCurrentIndexChanged: {
            artistImage.foreground = TTK_APP.artistImagePath().length === 0 ? "qrc:/image/widget_default_album_middle"
                                                                            : TTK_APP.artistImagePath();
            mediaArtistArea.source = TTK_APP.artistImagePath().length === 0 ? "qrc:/image/landscape_check_album_normal"
                                                                            : TTK_APP.artistImagePath();
            mediaAlbumtArea.source = mediaArtistArea.source;
            playBackgroundImage.source = TTK_APP.artistBgImagePath();

            musicPlayerShowTitle.text = TTK_APP.mediaName();
            musicPlayerShowArtist.text = "- " + TTK_APP.mediaArtist() + " -";
            ttkMusicPlayerCenterSettingPage.text = musicPlayerShowTitle.text;
            lovestMode.source = TTK_APP.checkLovestMusicSong() ? "qrc:/image/player_btn_favorited_normal"
                                                               : "qrc:/image/player_btn_favorite_normal"
        }
        onUpdateCurrentArtist: {
            artistImage.foreground = TTK_APP.artistImagePath().length === 0 ? "qrc:/image/widget_default_album_middle"
                                                                            : TTK_APP.artistImagePath();
            mediaArtistArea.source = TTK_APP.artistImagePath().length === 0 ? "qrc:/image/landscape_check_album_normal"
                                                                            : TTK_APP.artistImagePath();
            mediaAlbumtArea.source = mediaArtistArea.source;
        }
        onUpdateCurrentBgArtist: {
            playBackgroundImage.source = TTK_APP.artistBgImagePath();
        }
        onEmptyPlayerCenter: {
            if(state) {
                ttkOutStackView.pop();
            }
        }
    }

    onXChanged: {
        ttkMusicPlayerCenterSettingPage.visible = false;
        ttkMusicSongDownloadPage.visible = false;
        ttkMusicSongSharedPage.visible = false;
    }

    TTKMusicPlayerCenterSettingPage {
        id: ttkMusicPlayerCenterSettingPage
        text: TTK_APP.mediaName( TTK_APP.mediaName() );
    }

    TTKMusicSongDownloadPage {
        id: ttkMusicSongDownloadPage
        text: TTK_APP.mediaName( TTK_APP.mediaName() );
    }

    TTKMusicSongSharedPage {
        id: ttkMusicSongSharedPage
    }

    Rectangle {
        anchors.fill: parent
        Image {
            id: playBackgroundImage
            anchors.fill: parent
            source: TTK_APP.artistBgImagePath()
        }

        TTKBlurImage {
            anchors.fill: parent
            target: playBackgroundImage
        }
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: mainMenubar
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.color_alpha_lv12

            RowLayout {
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/player_btn_close_normal"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkOutStackView.pop();
                    }
                }

                TTKMarquee {
                    id: musicPlayerShowTitle
                    height: ttkGlobal.dpHeight(50)
                    width: ttkMusicPlayerCenter.width - ttkGlobal.dpHeight(100)
                    padding: 2
                    anchors.centerIn: parent
                    text: TTK_APP.mediaName()
                    color: ttkTheme.color_alpha_lv0
                    textColor: ttkTheme.color_white
                    fontSize: mainMenubar.height/2
                }

                TTKImageButton {
                    source: "qrc:/image/player_btn_more_normal"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.right: parent.right
                    onPressed: {
                        ttkMusicPlayerCenterSettingPage.visible = true;
                    }
                }
            }
        }

        ///main body
        TTKPageView {
            id: playCenterPageView
            Layout.fillWidth: true
            height: ttkMusicPlayerCenter.height - mainMenubar.height - playerComponent.height
            currentIndex: 1

            Rectangle {
                id: musicAlbumShow
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.color_alpha_lv12

                Rectangle {
                    width: ttkMusicPlayerCenter.width
                    height: 1
                    color: ttkTheme.color_gray
                }

                TTKImageFunctionItem {
                    id: mediaArtistArea
                    source: TTK_APP.artistImagePath().empty ? "qrc:/image/landscape_check_album_normal"
                                                            : TTK_APP.artistImagePath()
                    subSource: "qrc:/image/icon_folder_action_sheet"
                    text: qsTr("歌手: ") + TTK_APP.mediaArtist()

                }

                TTKImageFunctionItem {
                    id: mediaAlbumtArea
                    anchors.top: mediaArtistArea.bottom
                    source: TTK_APP.artistImagePath().empty ? "qrc:/image/landscape_check_album_normal"
                                                            : TTK_APP.artistImagePath()
                    subSource: "qrc:/image/icon_folder_action_sheet"
                    text: qsTr("专辑: ") + TTK_APP.mediaArtist()
                }
            }

            Rectangle {
                id: musicPlayerShow
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.color_alpha_lv12

                ColumnLayout {
                    spacing: 0
                    anchors.fill: parent

                    Text {
                        id: musicPlayerShowArtist
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredHeight: ttkGlobal.dpHeight(40)
                        Layout.preferredWidth: ttkMusicPlayerCenter.width - ttkGlobal.dpHeight(20)
                        color: ttkTheme.color_white
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        elide: Text.ElideMiddle
                        text: "- " + TTK_APP.mediaArtist() + " -"
                    }

                    TTKImageButton {
                        id: dtsEffectButton
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: ttkGlobal.dpWidth(50)
                        Layout.preferredHeight: ttkGlobal.dpHeight(30)
                        source: TTK_PLAYER.getMusicEnhanced() === 0 ? "qrc:/image/player_btn_dts_off"
                                                                    : "qrc:/image/player_btn_dts_on"
                        onPressed: {
                            ttkOutStackView.push("qrc:/MobileWidgets/TTKMusicDTSEffectPage.qml");
                        }
                    }

                    Rectangle {
                        id: musicPlayerShowCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: ttkGlobal.dpHeight(250)
                        color: ttkTheme.color_alpha_lv0

                        TTKRadiusImage {
                            id: artistImage
                            anchors.centerIn: parent;
                            width: parent.height
                            height: parent.height
                            color: ttkTheme.color_alpha_lv0
                            foreground: TTK_APP.artistImagePath().empty ? "qrc:/image/widget_default_album_middle"
                                                                        : TTK_APP.artistImagePath()
                            background: "qrc:/image/radius_big_mask"

                            RotationAnimation {
                                id: artistImageAnimation
                                target: artistImage
                                property: "rotation"
                                from: 0
                                to: 360
                                direction: RotationAnimation.Clockwise
                                duration: 16000
                                loops: Animation.Infinite
                            }
                        }
                    }

                    TTKMarquee {
                        id: musicPlayerShowLrc
                        Layout.alignment: Qt.AlignCenter
                        width: ttkMusicPlayerCenter.width
                        height: ttkGlobal.dpHeight(70)
                        textColor: ttkTheme.color_green
                        color: ttkTheme.color_alpha_lv0
                        fontSize: ttkGlobal.dpHeight(22)
                        text: TTK_LRC.value(musicLrcShow.currentIndex)
                    }
                }
            }

            Rectangle {
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.color_alpha_lv12

                TTKMusicLyricPage {
                    id: musicLrcShow
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height - ttkGlobal.dpHeight(40)
                    color: ttkTheme.color_alpha_lv0

                    Component.onCompleted: {
                        musicLrcShow.currentIndex = -1;
                    }
                }

                TTKImageButton {
                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                    }
                    width: ttkGlobal.dpWidth(40)
                    height: ttkGlobal.dpHeight(40)
                    source: "qrc:/image/player_btn_tan"
                }

                TTKImageButton {
                    anchors {
                        right: parent.right
                        bottom: parent.bottom
                    }
                    width: ttkGlobal.dpWidth(40)
                    height: ttkGlobal.dpHeight(40)
                    source: "qrc:/image/player_btn_lrc"
                    onPressed: {
                        console.log("clicked");
                    }
                }
            }
        }

        ///player component
        Rectangle {
            id: playerComponent
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(230)
            color: ttkTheme.color_alpha_lv12

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                ///timer component
                Rectangle {
                    Layout.fillWidth: true
                    height: ttkGlobal.dpHeight(50)
                    color: ttkTheme.color_alpha_lv0

                    RowLayout {
                        spacing: 10
                        anchors.fill: parent

                        Rectangle {
                            Layout.preferredWidth: ttkGlobal.dpWidth(5)
                        }

                        Text {
                            id: positionLabel
                            text: "00:00"
                            color: ttkTheme.color_white
                        }

                        Slider {
                            id: musicTimeSlider
                            Layout.fillWidth: true
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
                                        implicitHeight: ttkGlobal.dpHeight(3)
                                        color: ttkTheme.topbar_background
                                    }

                                    Rectangle{
                                        implicitWidth: musicTimeSlider.width*(1-musicTimeSlider.sliderGeometry())
                                        implicitHeight: ttkGlobal.dpHeight(3)
                                        color: ttkTheme.color_gray
                                    }
                                }

                                handle: Rectangle{
                                    anchors.centerIn: parent;
                                    color: ttkTheme.topbar_background
                                    width: ttkGlobal.dpWidth(20)
                                    height: ttkGlobal.dpHeight(20)
                                    radius: ttkGlobal.dpWidth(10)
                                }
                            }

                            MouseArea {
                                id: musicTimeSliderArea
                                anchors.fill: parent
                                onPressed: {
                                    var value = TTK_PLAYER.duration()/musicTimeSlider.width*mouse.x;
                                    musicTimeSlider.value = value;
                                    TTK_PLAYER.setPosition(value);
                                    TTK_LRC.setSongSpeedAndSlow(value);
                                }
                            }
                        }

                        Text {
                            id: durationLabel
                            text: "00:00"
                            color: ttkTheme.color_white
                        }

                        Rectangle {
                            Layout.preferredWidth: ttkGlobal.dpWidth(5)
                        }
                    }
                }

                ///control component
                Rectangle {
                    Layout.fillWidth: true
                    height: ttkGlobal.dpHeight(100)
                    color: ttkTheme.color_alpha_lv0

                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        Rectangle {
                            Layout.preferredWidth: ttkGlobal.dpWidth(20)
                        }

                        TTKImageButton {
                            id: playerPreButton
                            source: "qrc:/image/player_btn_pre_normal"
                            Layout.preferredWidth: ttkGlobal.dpWidth(120)
                            Layout.preferredHeight: ttkGlobal.dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                TTK_APP.playPrevious();
                                if(TTK_PLAYER.state() === 1) {
                                    changedPlayState();
                                }
                            }
                        }
                        TTKImageButton {
                            id: playerPlayButton
                            source: playStateChanged()
                            Layout.preferredWidth: ttkGlobal.dpWidth(120)
                            Layout.preferredHeight: ttkGlobal.dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                if(TTK_PLAYER.state() === 1) {
                                    playerPlayButton.source = "qrc:/image/player_btn_play_normal";
                                    TTK_PLAYER.pause();
                                    artistImageAnimation.pause();
                                }else {
                                    playerPlayButton.source = "qrc:/image/player_btn_pause_normal";
                                    TTK_PLAYER.play();
                                    artistImageAnimation.resume();
                                    artistImageAnimation.start();
                                }
                                ttkMusicBar.playStateChanged();
                            }
                        }

                        TTKImageButton {
                            id: playerNextButton
                            source: "qrc:/image/player_btn_next_normal"
                            Layout.preferredWidth: ttkGlobal.dpWidth(120)
                            Layout.preferredHeight: ttkGlobal.dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                TTK_APP.playNext();
                                if(TTK_PLAYER.state() === 1) {
                                    changedPlayState();
                                }
                            }
                        }

                        Rectangle {
                            Layout.preferredWidth: ttkGlobal.dpWidth(20)
                        }
                    }
                }

                ///other function component
                Rectangle {
                    Layout.fillWidth: true
                    height: ttkGlobal.dpHeight(80)
                    color: ttkTheme.color_alpha_lv0

                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        TTKImageButton {
                            id: lovestMode
                            source: TTK_APP.checkLovestMusicSong() ? "qrc:/image/player_btn_favorited_normal"
                                                                   : "qrc:/image/player_btn_favorite_normal"
                            Layout.preferredWidth: ttkGlobal.dpWidth(70)
                            Layout.preferredHeight: ttkGlobal.dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                TTK_APP.importLovestMusicSongs();
                                ttkFlyInOutBox.start();
                                if(TTK_APP.checkLovestMusicSong()) {
                                    source = "qrc:/image/player_btn_favorited_normal";
                                    ttkFlyInOutBox.text = qsTr("已加入到我的最爱")
                                }else {
                                    source = "qrc:/image/player_btn_favorite_normal";
                                    ttkFlyInOutBox.text = qsTr("已从我的最爱中移除")
                                }
                            }
                        }

                        TTKImageButton {
                            id: playerMode
                            source: "qrc:/image/player_btn_random_normal"
                            Layout.preferredWidth: ttkGlobal.dpWidth(70)
                            Layout.preferredHeight: ttkGlobal.dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkFlyInOutBox.start();
                                ++playMode;
                                if(playMode >= 5) {
                                    playMode = 2;
                                }
                                showPlayModeText(playMode);
                                TTK_APP.setPlaybackMode(playMode);
                            }
                        }

                        TTKImageButton {
                            source: "qrc:/image/player_btn_download_normal"
                            Layout.preferredWidth: ttkGlobal.dpWidth(70)
                            Layout.preferredHeight: ttkGlobal.dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkMusicSongDownloadPage.visible = true;
                            }
                        }

                        TTKImageButton {
                            source: "qrc:/image/player_btn_share_normal"
                            Layout.preferredWidth: ttkGlobal.dpWidth(70)
                            Layout.preferredHeight: ttkGlobal.dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkMusicSongSharedPage.visible = true;
                            }
                        }

                        TTKImageButton {
                            source: "qrc:/image/player_btn_playlist_normal"
                            Layout.preferredWidth: ttkGlobal.dpWidth(70)
                            Layout.preferredHeight: ttkGlobal.dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {

                            }
                        }
                    }
                }
            }
        }
    }

    TTKFlyInOutBox {
        id: ttkFlyInOutBox
    }
}
