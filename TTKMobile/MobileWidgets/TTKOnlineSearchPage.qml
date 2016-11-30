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

Item {
    id: ttkOnlineSearchPage
    width: parent.width
    height: parent.height

    function getSearchedText() {
        return searchInput.text.length === 0 ? searchInput.hint : searchInput.text;
    }

    Connections {
        target: TTK_NETWORK
        onClearAllItems: {
            searedSongList.currentIndex = -1;
            searedMVList.currentIndex = -1;
            searedLrcList.currentIndex = -1;

            searedSongListModel.clear();
            searedMVListModel.clear();
            searedLrcListModel.clear();
        }
        onCreateSearchedItems: {
            var info = { title: songname, artist: artistname };
            switch( functionList.currentIndex ) {
                case 0: searedSongListModel.append(info); break;
                case 1: searedMVListModel.append(info); break;
                case 2: searedLrcListModel.append(info); break;
            }
        }
        onDownLoadDataHasFinished: {
            loadingImageAnimation.stopLoading();
            if(empty) {
                noMusicFoundItem.visible = true;
            }
        }
        onDownForSearchSongFinished: {
            TTK_APP.importNetworkMusicSongs(key, path);
        }
        onDownForSearchMovieFinished: {
            ttkGlobal.video_url = url;
            ttkOutStackView.push("qrc:/MobileWidgets/TTKMusicVideoPage.qml");
        }
    }

    TTKMusicSongDownloadPage {
        id: ttkMusicSongDownloadPage
        autoDownloadFlag: false
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: mainMenubar
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.topbar_background

            RowLayout {
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkMainStackView.pop();
                    }
                }

                TTKLineInput {
                    id: searchInput
                    Layout.preferredWidth: ttkOnlineSearchPage.width - ttkGlobal.dpWidth(100)
                    Layout.preferredHeight: ttkGlobal.dpHeight(33)
                    hint: "Big Bang"
                    fontSize: parent.height/3
                }

                TTKTextButton {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.right: parent.right
                    textColor: ttkTheme.color_white
                    text: qsTr("搜索")
                    onPressed: {
                        loadingImageAnimation.startLoading();
                        switch( functionList.currentIndex ) {
                            case 0: TTK_NETWORK.searchSong(getSearchedText()); break;
                            case 1: TTK_NETWORK.searchMovie(getSearchedText()); break;
                            case 2: TTK_NETWORK.searchLrc(getSearchedText()); break;
                        }
                    }
                }
            }
        }

        ///function bar
        Rectangle {
            id: searchTypeArea
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.color_white

            ListView {
                id: functionList
                anchors.fill: parent
                orientation: ListView.Horizontal
                boundsBehavior: Flickable.StopAtBounds
                spacing: 0
                clip: true

                delegate: Component {
                    TTKTextButton {
                        width: ttkMainWindow.width/5
                        height: ttkGlobal.dpHeight(50)
                        textColor: ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.color_gray
                        text: title
                        onPressed: {
                            searedSongStackView.pop();
                            switch( functionList.currentIndex = index ) {
                                case 0:
                                    searedSongStackView.push(searedSongList);
                                    TTK_NETWORK.searchSong(getSearchedText())
                                    loadingImageAnimation.startLoading();
                                    break;
                                case 1:
                                    searedSongStackView.push(searedMVList);
                                    TTK_NETWORK.searchMovie(getSearchedText());
                                    loadingImageAnimation.startLoading();
                                    break;
                                case 2:
                                    searedSongStackView.push(searedLrcList);
                                    TTK_NETWORK.searchLrc(getSearchedText());
                                    loadingImageAnimation.startLoading();
                                    break;
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: ttkGlobal.dpHeight(5)
                            anchors.bottom: parent.bottom
                            color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.color_white
                        }
                    }
                }

                model: ListModel {
                    ListElement { title: qsTr("单曲")}
                    ListElement { title: qsTr("MV")}
                    ListElement { title: qsTr("歌词")}
                    ListElement { title: qsTr("专辑")}
                    ListElement { title: qsTr("歌单")}
                }
            }
        }

        ///main body
        Rectangle {
            width: ttkMainWindow.width
            height: ttkMainStackView.height - mainMenubar.height - searchTypeArea.height
            color: ttkTheme.color_white

            StackView {
                id: searedSongStackView
                width: parent.width
                height: parent.height
                focus: true
                initialItem: searedSongList

                ///seared song list
                ListView {
                    id: searedSongList
                    width: parent.width
                    height: parent.height
                    visible: false
                    clip: true

                    delegate: Component {
                        Rectangle {
                            id: wrapperSong
                            width: ttkMainWindow.width
                            height: ttkGlobal.dpHeight(70)
                            color: ttkTheme.color_white

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    searedSongList.currentIndex = index;
                                    TTK_NETWORK.setCurrentIndex(index);
                                }
                            }

                            Rectangle {
                                width: ttkMainWindow.width
                                height: 1
                                color: ttkTheme.color_alpha_lv9
                            }

                            Rectangle {
                                width: ttkGlobal.dpWidth(5)
                                height: parent.height*2/3
                                anchors {
                                    top: parent.top
                                    topMargin: parent.height/3/2
                                }
                                color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.color_white
                            }

                            Text {
                                id: titleAreaSong
                                text: title
                                width: ttkOnlineSearchPage.width - iconAreaSong.width - ttkGlobal.dpHeight(120)
                                anchors {
                                    top: parent.top
                                    topMargin: ttkGlobal.dpHeight(10)
                                    left: parent.left
                                    leftMargin: ttkGlobal.dpHeight(20)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height*3/10
                            }

                            Image {
                                id: iconAreaSong
                                width: parent.height/3
                                height: parent.height/3
                                anchors {
                                    top: titleAreaSong.bottom
                                    topMargin: ttkGlobal.dpHeight(5)
                                    left: parent.left
                                    leftMargin: ttkGlobal.dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_normal"
                            }

                            Text {
                                text: artist
                                width: titleAreaSong.width - iconAreaSong.width
                                anchors {
                                    top: titleAreaSong.bottom
                                    topMargin: ttkGlobal.dpHeight(10)
                                    left: iconAreaSong.right
                                    leftMargin: ttkGlobal.dpHeight(10)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height/4
                                color: ttkTheme.color_gray
                            }

                            TTKImageButton {
                                width: parent.height/2
                                height: parent.height/2
                                anchors {
                                    top: parent.top
                                    right: parent.right
                                    topMargin: ttkGlobal.dpHeight(20)
                                    rightMargin: ttkGlobal.dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_more_normal"
                                onPressed: {
                                    ttkMusicSongDownloadPage.queryType = ttkTheme.search_type_download_song_index;
                                    ttkMusicSongDownloadPage.songIndex = index;
                                    ttkMusicSongDownloadPage.jsonAtrrString = TTK_NETWORK.getSearchedAttributes(index);
                                    ttkMusicSongDownloadPage.visible = true;
                                }
                            }
                        }
                    }

                    model: ListModel {
                        id: searedSongListModel
                    }
                }

                ///seared mv list
                ListView {
                    id: searedMVList
                    width: parent.width
                    height: parent.height
                    visible: false
                    clip: true

                    delegate: Component {
                        Rectangle {
                            id: wrapperMV
                            width: ttkMainWindow.width
                            height: ttkGlobal.dpHeight(70)
                            color: ttkTheme.color_white

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    searedMVList.currentIndex = index;
                                    TTK_NETWORK.setCurrentIndex(index);
                                }
                            }

                            Rectangle {
                                width: ttkMainWindow.width
                                height: 1
                                color: ttkTheme.color_alpha_lv9
                            }

                            Rectangle {
                                width: ttkGlobal.dpWidth(5)
                                height: parent.height*2/3
                                anchors {
                                    top: parent.top
                                    topMargin: parent.height/3/2
                                }
                                color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.color_white
                            }

                            Text {
                                id: titleAreaMV
                                text: title
                                width: ttkOnlineSearchPage.width - iconAreaMV.width - ttkGlobal.dpHeight(120)
                                anchors {
                                    top: parent.top
                                    topMargin: ttkGlobal.dpHeight(10)
                                    left: parent.left
                                    leftMargin: ttkGlobal.dpHeight(20)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height*3/10
                            }

                            Image {
                                id: iconAreaMV
                                width: parent.height/3
                                height: parent.height/3
                                anchors {
                                    top: titleAreaMV.bottom
                                    topMargin: ttkGlobal.dpHeight(5)
                                    left: parent.left
                                    leftMargin: ttkGlobal.dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_normal"
                            }

                            Text {
                                text: artist
                                width: titleAreaMV.width - iconAreaMV.width
                                anchors {
                                    top: titleAreaMV.bottom
                                    topMargin: ttkGlobal.dpHeight(10)
                                    left: iconAreaMV.right
                                    leftMargin: ttkGlobal.dpHeight(10)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height/4
                                color: ttkTheme.color_gray
                            }

                            TTKImageButton {
                                width: parent.height/2
                                height: parent.height/2
                                anchors {
                                    top: parent.top
                                    right: parent.right
                                    topMargin: ttkGlobal.dpHeight(20)
                                    rightMargin: ttkGlobal.dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_more_normal"
                                onPressed: {
                                    ttkMusicSongDownloadPage.queryType = ttkTheme.search_type_download_mv_index;
                                    ttkMusicSongDownloadPage.songIndex = index;
                                    ttkMusicSongDownloadPage.jsonAtrrString = TTK_NETWORK.getSearchedAttributes(index);
                                    ttkMusicSongDownloadPage.visible = true;
                                }
                            }
                        }
                    }

                    model: ListModel {
                        id: searedMVListModel
                    }
                }

                ///seared lrc list
                ListView {
                    id: searedLrcList
                    width: parent.width
                    height: parent.height
                    visible: false
                    clip: true

                    delegate: Component {
                        Rectangle {
                            id: wrapperLrc
                            width: ttkMainWindow.width
                            height: ttkGlobal.dpHeight(70)
                            color: ttkTheme.color_white

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    searedLrcList.currentIndex = index;
                                    TTK_NETWORK.setCurrentIndex(index);
                                    ttkFlyInOutBox.text = qsTr("已加入下载列表");
                                    ttkFlyInOutBox.start();
                                }
                            }

                            Rectangle {
                                width: ttkMainWindow.width
                                height: 1
                                color: ttkTheme.color_alpha_lv9
                            }

                            Rectangle {
                                width: ttkGlobal.dpWidth(5)
                                height: parent.height*2/3
                                anchors {
                                    top: parent.top
                                    topMargin: parent.height/3/2
                                }
                                color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.color_white
                            }

                            Text {
                                id: titleAreaLrc
                                text: title
                                width: ttkOnlineSearchPage.width - iconAreaLrc.width - ttkGlobal.dpHeight(120)
                                anchors {
                                    top: parent.top
                                    topMargin: ttkGlobal.dpHeight(10)
                                    left: parent.left
                                    leftMargin: ttkGlobal.dpHeight(20)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height*3/10
                            }

                            Image {
                                id: iconAreaLrc
                                width: parent.height/3
                                height: parent.height/3
                                anchors {
                                    top: titleAreaLrc.bottom
                                    topMargin: ttkGlobal.dpHeight(5)
                                    left: parent.left
                                    leftMargin: ttkGlobal.dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_normal"
                            }

                            Text {
                                text: artist
                                width: titleAreaLrc.width - iconAreaLrc.width
                                anchors {
                                    top: titleAreaLrc.bottom
                                    topMargin: ttkGlobal.dpHeight(10)
                                    left: iconAreaLrc.right
                                    leftMargin: ttkGlobal.dpHeight(10)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height/4
                                color: ttkTheme.color_gray
                            }
                        }
                    }

                    model: ListModel {
                        id: searedLrcListModel
                    }
                }

            }

            RotationAnimation {
                id: loadingImageAnimation
                target: loadingImage
                property: "rotation"
                from: 0
                to: 360
                direction: RotationAnimation.Clockwise
                duration: 2000
                loops: Animation.Infinite

                function startLoading() {
                    noMusicFoundItem.visible = false;
                    loadingImage.visible = true;
                    loadingImageAnimation.start();
                }

                function stopLoading() {
                    noMusicFoundItem.visible = false;
                    loadingImage.visible = false;
                    loadingImageAnimation.stop();
                }
            }

            Image {
                id: loadingImage
                visible: false
                source: "qrc:/image/data_searching"
                anchors.centerIn: parent
            }

            TTKMainFunctionItem {
                id: noMusicFoundItem
                visible: false
                anchors.centerIn: parent
                source: "qrc:/image/ic_start_recognize_bottom"
                mainTitle: qsTr("空空如也")
                subTitle: qsTr("未搜索到相关歌曲")
                mainTitleSize: ttkGlobal.dpHeight(150)/8
            }
        }
    }

    TTKFlyInOutBox {
        id: ttkFlyInOutBox
    }
}
