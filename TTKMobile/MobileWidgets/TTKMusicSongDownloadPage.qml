/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import "Core"

Rectangle {
    id: ttkMusicSongDownloadPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    property bool autoDownloadFlag: true
    property int songBitrate: -1
    property int songIndex: -1
    property int queryType: 0
    property string text
    property string jsonAtrrString

    function clearData() {
        itemListModel.clear();
        itemListView.currentIndex = -1;
    }

    function createData(bitrate) {
        downloadButton.enabled = true;
        itemListView.currentIndex = -1;
        var bitrateString;

        if(queryType !== ttkTheme.search_type_download_mv_index) {
            switch(bitrate) {
                case -1:  bitrateString = qsTr("没有搜到任何结果"); break;
                case 128: bitrateString = qsTr("标准品质"); break;
                case 192: bitrateString = qsTr("高品质"); break;
                case 320: bitrateString = qsTr("超高品质"); break;
                case 1000:bitrateString = qsTr("无损品质"); break;
                default: break;
            }
        }else {
            switch(bitrate) {
                case -1:  bitrateString = qsTr("没有搜到任何结果"); break;
                case 500: bitrateString = qsTr("高清品质"); break;
                case 750: bitrateString = qsTr("超清品质"); break;
                default: break;
            }
        }

        if(bitrateString.length !== 0) {
            itemListModel.append({title: bitrateString, bit: bitrate});
        }
    }

    Connections {
        target: TTK_NETWORK
        onClearAllItems: {
            clearData();
        }
        onCreateDownloadSongQuality: {
            createData(bitrate)
        }
        onDownForDownloadSongFinished: {
            if(path.length !== 0) {
                TTK_APP.importDownloadMusicSongs(ttkTheme.music_download_list, path);
            }
        }
        onDownForDownloadMovieFinished: {
            if(path.length !== 0) {
                TTK_APP.importDownloadMusicSongs(ttkTheme.music_downmv_list, path);
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
        height: ttkGlobal.dpHeight(60) + itemListView.height
        color: "#F0F0F0"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ListView {
            id: itemListView
            width: parent.width
            height: ttkGlobal.dpHeight(60)*itemListView.count
            spacing: 0
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                width: itemListView.width
                height: ttkGlobal.dpHeight(60)
                Text {
                    anchors {
                        fill: parent
                        left: parent.left
                        leftMargin: ttkGlobal.dpHeight(20)
                    }
                    verticalAlignment: Qt.AlignVCenter
                    width: parent.width
                    text: title
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: ttkTheme.color_alpha_lv9
                }

                Image {
                    source: parent.ListView.isCurrentItem ? "qrc:/image/ic_playlist_normal" : ""
                    width: ttkGlobal.dpWidth(30)
                    height: ttkGlobal.dpHeight(30)
                    anchors {
                        right: parent.right
                        rightMargin: ttkGlobal.dpHeight(20)
                        top: parent.top
                        topMargin: ttkGlobal.dpHeight(15)
                        bottom: parent.bottom
                        bottomMargin: ttkGlobal.dpHeight(15)
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
            color: ttkTheme.color_alpha_lv10
        }

        TTKTextButton {
            id: downloadButton
            anchors {
                top: itemListView.bottom
                left: parent.left
            }
            width: parent.width
            height: ttkGlobal.dpHeight(60)
            textColor: ttkTheme.color_black
            text: qsTr("下载")
            textSize: ttkMusicSongDownloadPage.height/25
            enabled: false
            onPressed: {
                if(songBitrate > 0) {
                    ttkMusicSongDownloadPage.visible = false;
                    TTK_NETWORK.setCurrentIndex(songIndex, songBitrate);

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
            if(autoDownloadFlag) {
                TTK_NETWORK.downloadSong(text);
            }else {
                TTK_NETWORK.setQueryType(queryType);
                var json = JSON.parse(jsonAtrrString);
                if(json.length !== 0) {
                    clearData();
                }
                for(var i=0; i<json.length; ++i) {
                    createData( json[i].bitrate );
                }
            }
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
