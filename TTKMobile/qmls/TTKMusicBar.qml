import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Rectangle {
    id: ttkMusicBar

    property alias nameTitle: musicNameTitle.text
    property alias artistTitle: musicArtistTitle.text

    function playStateChanged() {
        if(ttkMusicPlayer.state() === 1) {
            barPlayButton.source = "qrc:/image/landscape_player_btn_pause_normal";
            musicBarImageAnimation.resume();
            musicBarImageAnimation.start();
        }else{
            barPlayButton.source = "qrc:/image/landscape_player_btn_play_normal";
            musicBarImageAnimation.pause();
        }
    }

    anchors.right: ttkMainWindow.right
    anchors.bottom: ttkMainWindow.bottom
    width: ttkMainWindow.width
    height: dpHeight(70)
    border {
        color: ttkTheme.alphaLv9
        width: 1
    }
    color: ttkTheme.white

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            if(ttkMusicPlaylist.currentIndex() !== -1) {
                ttkOutStackView.push("qrc:/qmls/TTKMusicPlayerCenterPage.qml");
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
            leftMargin: dpHeight(10)
        }
        color: ttkTheme.alphaLv0
        foreground: "qrc:/image/landscape_check_album_normal"
        background: "qrc:/image/radius_mask"

        RotationAnimation {
            id: musicBarImageAnimation
            target: musicBarImage
            property: "rotation"
            from: 0
            to: 360
            direction: RotationAnimation.Clockwise
            duration: 4000
            loops: Animation.Infinite
        }
    }

    Text {
        id: musicNameTitle
        anchors {
            left: musicBarImage.right
            top: parent.top
            leftMargin: dpHeight(10)
            topMargin: dpHeight(10)
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: parent.height/4
        text: "天天酷音"
    }

    Text {
        id: musicArtistTitle
        anchors {
            left: musicBarImage.right
            top: musicNameTitle.bottom
            leftMargin: dpHeight(10)
            topMargin: dpHeight(10)
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "gray"
        text: "天天酷音"
    }

    TTKImageButton {
        id: barPlayListButton
        source: "qrc:/image/minibar_btn_playlist_normal"
        anchors {
            right: parent.right
            top: parent.top
            topMargin: (parent.height - height)/2
            rightMargin: dpHeight(10)
        }
        width: height
        height: dpHeight(60)
    }

    TTKImageButton {
        id: barPlayButton
        source: "qrc:/image/landscape_player_btn_play_normal"
        anchors {
            right: barPlayListButton.left
            top: parent.top
            topMargin: (parent.height - height)/2
            rightMargin: dpHeight(10)
        }
        width: height*1.2
        height: dpHeight(50)
        onPressed: {
            if(ttkMusicPlaylist.currentIndex() === -1) {
                return;
            }
            if(ttkMusicPlayer.state() === 1) {
                barPlayButton.source = "qrc:/image/landscape_player_btn_play_normal";
                ttkMusicPlayer.pause();
                musicBarImageAnimation.pause();
            }else{
                barPlayButton.source = "qrc:/image/landscape_player_btn_pause_normal";
                ttkMusicPlayer.play();
                musicBarImageAnimation.resume();
                musicBarImageAnimation.start();
            }
        }
    }
}
