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

    function playStateChanged() {
        if(TTK_PLAYER.state() === 1) {
            playerPlayButton.source = "qrc:/image/player_btn_pause_normal";
            artistImageAnimation.resume();
            artistImageAnimation.start();
        }else{
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
            musicTimeSlider.maximumValue = TTK_PLAYER.duration();
            musicTimeSlider.value = position;
            durationLabel.text = TTK_UTILS.normalizeTime(TTK_PLAYER.duration(), "mm:ss");
            positionLabel.text = TTK_UTILS.normalizeTime(position, "mm:ss");
        }
    }

    Connections {
        target: TTK_APP
        onCurrentIndexChanged: {
            musicPlayerShowTitle.text = TTK_APP.mediaName(index);
            musicPlayerShowArtist.text = "- " + TTK_APP.mediaArtist(index) + " -";
        }
    }

    Rectangle {
        anchors.fill: parent
        Image {
            id: playBackgroundImage
            anchors.fill: parent
            source: "qrc:/image/test"
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
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.alphaLv14

            RowLayout {
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/player_btn_close_normal"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkOutStackView.pop();
                    }
                }

                Text {
                    id: musicPlayerShowTitle
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: ttkMusicPlayerCenter.width - dpHeight(100)
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignHCenter
                    font.pixelSize: mainMenubar.height/2
                    color: ttkTheme.white
                    elide: Text.ElideRight
                    text: TTK_APP.mediaName()
                }

                TTKImageButton {
                    source: "qrc:/image/player_btn_more_normal"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.right: parent.right
                    onPressed: {

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
                color: ttkTheme.alphaLv14

                Rectangle {
                    width: ttkMusicPlayerCenter.width
                    height: 1
                    color: "gray"
                }

                TTKImageFunctionItem {
                    id: mediaArtistArea
                    source: "qrc:/image/test"
                    text: qsTr("歌手: ") + TTK_APP.mediaArtist()
                }

                TTKImageFunctionItem {
                    id: mediaAlbumtArea
                    anchors.top: mediaArtistArea.bottom
                    source: "qrc:/image/test"
                    text: qsTr("专辑: ") + TTK_APP.mediaArtist()
                }
            }

            Rectangle {
                id: musicPlayerShow
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.alphaLv14

                ColumnLayout {
                    spacing: 0
                    anchors.fill: parent

                    Text {
                        id: musicPlayerShowArtist
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredHeight: dpHeight(50)
                        Layout.preferredWidth: ttkMusicPlayerCenter.width - dpHeight(20)
                        color: ttkTheme.white
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        elide: Text.ElideMiddle
                        text: "- " + TTK_APP.mediaArtist() + " -"
                    }

                    Rectangle {
                        id: musicPlayerShowCenter
                        Layout.fillWidth: true
                        Layout.preferredHeight: dpHeight(270)
                        color: ttkTheme.alphaLv0

                        TTKRadiusImage {
                            id: artistImage
                            anchors.centerIn: parent;
                            width: parent.height
                            height: parent.height
                            color: ttkTheme.alphaLv0
                            foreground: "qrc:/image/widget_default_album_middle"
                            background: "qrc:/image/radius_big_mask"

                            RotationAnimation {
                                id: artistImageAnimation
                                target: artistImage
                                property: "rotation"
                                from: 0
                                to: 360
                                direction: RotationAnimation.Clockwise
                                duration: 8000
                                loops: Animation.Infinite
                            }
                        }
                    }

                    Rectangle {
                        id: musicPlayerShowLrc
                        Layout.fillWidth: true
                        Layout.preferredHeight: dpHeight(80)
                        color: ttkTheme.alphaLv0
                    }
                }
            }

            Rectangle {
                id: musicLrcShow
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.alphaLv14
            }
        }

        ///player component
        Rectangle {
            id: playerComponent
            Layout.fillWidth: true
            height: dpHeight(230)
            color: ttkTheme.alphaLv14

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                ///timer component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(50)
                    color: ttkTheme.alphaLv0

                    RowLayout {
                        spacing: 10
                        anchors.fill: parent

                        Rectangle {
                            Layout.preferredWidth: dpWidth(5)
                        }

                        Text {
                            id: positionLabel
                            text: "00:00"
                            color: ttkTheme.white
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
                                        implicitWidth: musicTimeSlider.width*musicTimeSlider.sliderGeometry();
                                        implicitHeight: dpHeight(3)
                                        color: ttkTheme.topbar_background
                                    }

                                    Rectangle{
                                        implicitWidth: musicTimeSlider.width*(1 - musicTimeSlider.sliderGeometry());
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

                        Text {
                            id: durationLabel
                            text: "00:00"
                            color: ttkTheme.white
                        }

                        Rectangle {
                            Layout.preferredWidth: dpWidth(5)
                        }
                    }
                }

                ///control component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(100)
                    color: ttkTheme.alphaLv0

                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        Rectangle {
                            Layout.preferredWidth: dpWidth(20)
                        }

                        TTKImageButton {
                            id: playerPreButton
                            source: "qrc:/image/player_btn_pre_normal"
                            Layout.preferredWidth: dpWidth(120)
                            Layout.preferredHeight: dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                TTK_APP.playPrevious();
                            }
                        }
                        TTKImageButton {
                            id: playerPlayButton
                            source: playStateChanged()
                            Layout.preferredWidth: dpWidth(120)
                            Layout.preferredHeight: dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                if(TTK_PLAYER.state() === 1) {
                                    playerPlayButton.source = "qrc:/image/player_btn_play_normal";
                                    TTK_PLAYER.pause();
                                    artistImageAnimation.pause();
                                }else{
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
                            Layout.preferredWidth: dpWidth(120)
                            Layout.preferredHeight: dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                TTK_APP.playNext();
                            }
                        }

                        Rectangle {
                            Layout.preferredWidth: dpWidth(20)
                        }
                    }
                }

                ///other function component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(80)
                    color: ttkTheme.alphaLv0

                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        TTKImageButton {
                            source: "qrc:/image/player_btn_favorite_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKImageButton {
                            id: playerMode
                            source: "qrc:/image/player_btn_random_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
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
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKImageButton {
                            source: "qrc:/image/player_btn_share_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKImageButton {
                            source: "qrc:/image/player_btn_playlist_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
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
