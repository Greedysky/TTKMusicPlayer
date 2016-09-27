import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Rectangle {
    id: ttkMusicBar
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
        onPressed: {
            ttkOutStackView.push("qrc:/qmls/TTKMusicPlayerCenterPage.qml");
        }
    }

    TTKRadiusImage {
        id: radioImage
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
    }

    Text {
        id: radioMainTitle
        anchors {
            left: radioImage.right
            top: parent.top
            leftMargin: dpHeight(10)
            topMargin: dpHeight(10)
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: parent.height/4
        text: "个性电台"
    }

    Text {
        id: radioSubTitle
        anchors {
            left: radioImage.right
            top: radioMainTitle.bottom
            leftMargin: dpHeight(10)
            topMargin: dpHeight(10)
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        color: "gray"
        text: "偶遇身边好音乐"
    }

    TTKButton {
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

    TTKButton {
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
            barPlayButton.source = "qrc:/image/landscape_player_btn_pause_normal"
        }
    }
}
