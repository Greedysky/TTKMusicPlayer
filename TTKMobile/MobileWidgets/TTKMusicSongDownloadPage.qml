/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

import QtQuick 2.5
import "Core"

Rectangle {
    id: ttkMusicSongDownloadPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    property int songBitrate: -1
    property int songIndex: -1
    property int queryType: 0
    property string text
    property string jsonAtrrString

    function startSearch() {
        clearData();
        itemListModel.append({title: qsTr("正在获取数据当中..."), bit: -1});
        if(visible === true) {
            verticalYAnimation.start();
            TTK_NETWORK.setQueryType(queryType);
            var json = JSON.parse(jsonAtrrString);
            if(json.length !== 0) {
                clearData();
            }else{
                createData(-1);
            }

            for(var i=0; i<json.length; ++i) {
                createData(json[i]);
            }
        }
    }

    function clearData() {
        itemListModel.clear();
        itemListView.currentIndex = -1;
    }

    function songInfoFormat(songInfo) {
        return "(" + songInfo.bitrate + "kbps/"+ songInfo.size + "/"+ songInfo.format + ")";
    }

    function createData(songInfo) {
        downloadButton.enabled = true;
        itemListView.currentIndex = -1;
        var bitrateString;

        if(songInfo.bitrate < 0)
            bitrateString = qsTr("没有搜到任何结果");
        else if(0 < songInfo.bitrate && songInfo.bitrate <= 128)
            bitrateString = qsTr("标准品质") + songInfoFormat(songInfo);
        else if(128 < songInfo.bitrate && songInfo.bitrate <= 192)
            bitrateString = qsTr("高品质") + songInfoFormat(songInfo);
        else if(192 < songInfo.bitrate && songInfo.bitrate <= 320)
            bitrateString = qsTr("超高品质") + songInfoFormat(songInfo);
        else if(songInfo.bitrate > 320)
            bitrateString = qsTr("无损品质") + songInfoFormat(songInfo);

        if(bitrateString.length !== 0) {
            itemListModel.append({title: bitrateString, bit: songInfo.bitrate});
        }
    }

    Connections {
        target: TTK_NETWORK
        onClearAllItems: {
            clearData();
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
                    onClicked: {
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
            onClicked: {
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
        startSearch();
    }

    Component.onCompleted: {
        itemListView.currentIndex = -1;
        var docRoot = ttkMusicSongDownloadPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicSongDownloadPage.parent = docRoot;
    }
}
