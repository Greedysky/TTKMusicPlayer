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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "Core"

Item {
    id: ttkMusicOnlineSearchPage
    width: parent.width
    height: parent.height

    function getSearchedText() {
        return searchInput.text.length === 0 ? searchInput.hint : searchInput.text;
    }

    Connections {
        target: TTK_NETWORK
        onClearAllItems: {
            searedSongList.currentIndex = -1;

            searedSongListModel.clear();
        }
        onCreateSearchedItem: {
            var info = { title: songname, artist: artistname };
            switch( functionList.currentIndex ) {
                case 0: searedSongListModel.append(info); break;
            }
        }
        onQueryDataHasFinished: {
            loadingImageAnimation.stopLoading();
            if(empty) {
                noMusicFoundItem.visible = true;
            }
        }
    }

    TTKMusicSongDownloadPage {
        id: ttkMusicSongDownloadPage
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
                    onClicked: {
                        TTK_NETWORK.abort();
                        ttkMainStackView.pop();
                    }
                }

                TTKLineInput {
                    id: searchInput
                    Layout.preferredWidth: ttkMusicOnlineSearchPage.width - ttkGlobal.dpWidth(100)
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
                    onClicked: {
                        loadingImageAnimation.startLoading();
                        switch( functionList.currentIndex ) {
                            case 0: TTK_NETWORK.searchSong(getSearchedText()); break;
                            case 1: TTK_NETWORK.searchMovie(getSearchedText()); break;
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
                        onClicked: {
                            searedSongStackView.pop();
                            switch( functionList.currentIndex = index ) {
                                case 0:
                                    searedSongStackView.push(searedSongList);
                                    TTK_NETWORK.searchSong(getSearchedText())
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
                                    ttkMusicSongDownloadPage.queryType = ttkTheme.search_type_download_song_index;
                                    ttkMusicSongDownloadPage.songIndex = index;
                                    ttkMusicSongDownloadPage.jsonAtrrString = TTK_NETWORK.getSearchedAttributes(index);
                                    ttkMusicSongDownloadPage.visible = true;
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
                                width: ttkMusicOnlineSearchPage.width - iconAreaSong.width - ttkGlobal.dpHeight(120)
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
                                onClicked: {
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
