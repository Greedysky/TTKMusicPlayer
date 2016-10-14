/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.4
import "Core"

Rectangle {
    id: ttkMusicSongDownloadPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.alphaLv12

    property int songBitrate: -1
    property string text

    function clearData() {
        itemListModel.clear();
        itemListView.currentIndex = -1;
    }

    Connections {
        target: TTK_NETWORK
        onClearAllItems: {
            clearData();
        }

        onCreateDownloadSongQuality: {
            downloadButton.enabled = true;
            itemListView.currentIndex = -1;
            var bitrateString;

            switch(bitrate)
            {
                case -1:  bitrateString = qsTr("没有搜到任何结果"); break;
                case 128: bitrateString = qsTr("标准品质"); break;
                case 192: bitrateString = qsTr("高品质"); break;
                case 320: bitrateString = qsTr("超高品质"); break;
                case 1000:bitrateString = qsTr("无损品质"); break;
                default: break;
            }

            if(bitrateString.length !== 0) {
                itemListModel.append({title: bitrateString, bit: bitrate});
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicSongDownloadPage.visible = false;
        }
    }

    Rectangle {
        width: parent.width
        height: dpHeight(60) + itemListView.height
        color: "#F0F0F0"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ListView {
            id: itemListView
            width: parent.width
            height: dpHeight(60)*itemListView.count
            spacing: 0
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                width: itemListView.width
                height: dpHeight(60)
                Text {
                    anchors {
                        fill: parent
                        left: parent.left
                        leftMargin: dpHeight(20)
                    }
                    verticalAlignment: Qt.AlignVCenter
                    width: parent.width
                    text: title
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: ttkTheme.alphaLv9
                }

                Image {
                    source: parent.ListView.isCurrentItem ? "qrc:/image/ic_playlist_normal" : ""
                    width: dpWidth(30)
                    height: dpHeight(30)
                    anchors {
                        right: parent.right
                        rightMargin: dpHeight(20)
                        top: parent.top
                        topMargin: dpHeight(15)
                        bottom: parent.bottom
                        bottomMargin: dpHeight(15)
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        itemListView.currentIndex = index;
                        songBitrate = bit;
                    }
                }
            }

            model: ListModel {
                id: itemListModel
            }
        }

        Rectangle {
            anchors {
                top: itemListView.bottom
                left: parent.left
            }
            width: parent.width
            height: 1
            color: ttkTheme.alphaLv10
        }

        TTKTextButton {
            id: downloadButton
            anchors {
                top: itemListView.bottom
                left: parent.left
            }
            width: parent.width
            height: dpHeight(60)
            textColor: ttkTheme.black
            text: qsTr("下载")
            textSize: ttkMusicSongDownloadPage.height/25
            enabled: false
            onPressed: {
                if(songBitrate > 0) {
                    TTK_NETWORK.setCurrentIndex(-1, songBitrate);
                    ttkMusicSongDownloadPage.visible = false;
                    ttkFlyInOutBox.text = qsTr("已加入下载列表");
                    ttkFlyInOutBox.start();
                }
            }
        }
    }

    NumberAnimation {
        id: verticalYAnimation
        property: "y"
        target: ttkMusicSongDownloadPage
        from: ttkMusicSongDownloadPage.height
        to: 0
        duration: 200
    }

    onVisibleChanged: {
        clearData();
        itemListModel.append({title: qsTr("正在获取数据当中..."), bit: -1});
        if(visible === true) {
            verticalYAnimation.start();
            TTK_NETWORK.downloadSong(text);
        }
    }

    Component.onCompleted:
    {
        itemListView.currentIndex = -1;
        var docRoot = ttkMusicSongDownloadPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicSongDownloadPage.parent = docRoot;
    }
}
