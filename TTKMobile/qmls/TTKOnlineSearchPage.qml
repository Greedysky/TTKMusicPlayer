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
            searedSongListModel.clear();
            searedMVListModel.clear();
            searedLrcListModel.clear();
        }
        onCreateSearchedItems: {
            var info = { title: songname, artist: artistname };
            switch( functionList.currentIndex )
            {
                case 0: searedSongListModel.append(info); break;
                case 1: searedMVListModel.append(info); break;
                case 2: searedLrcListModel.append(info); break;
            }
        }
        onDownForSearchSongFinished: {
            TTK_APP.importNetworkMusicSongs(key, path);
        }
        onDownForSearchMovieFinished: {
            console.log(url);
//            ttkOutStackView.push("qrc:/qmls/TTKMusicVideoPage.qml");
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
            color: ttkTheme.topbar_background

            RowLayout {
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkMainStackView.pop();
                    }
                }

                TTKLineInput {
                    id: searchInput
                    Layout.preferredWidth: ttkOnlineSearchPage.width - dpWidth(100)
                    Layout.preferredHeight: dpHeight(33)
                    hint: "Big Bang"
                    fontSize: parent.height/3
                }

                TTKTextButton {
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.right: parent.right
                    textColor: ttkTheme.white
                    text: qsTr("搜索")
                    onPressed: {
                        switch( functionList.currentIndex )
                        {
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
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.white

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
                        height: dpHeight(50)
                        textColor: ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.gray
                        text: title
                        onPressed: {
                            searedSongStackView.pop();
                            switch( functionList.currentIndex = index )
                            {
                                case 0:
                                    searedSongStackView.push(searedSongList);
                                    TTK_NETWORK.searchSong(getSearchedText())
                                    break;
                                case 1:
                                    searedSongStackView.push(searedMVList);
                                    TTK_NETWORK.searchMovie(getSearchedText());
                                    break;
                                case 2:
                                    searedSongStackView.push(searedLrcList);
                                    TTK_NETWORK.searchLrc(getSearchedText());
                                    break;
                            }
                        }

                        Rectangle {
                            width: parent.width
                            height: dpHeight(5)
                            anchors.bottom: parent.bottom
                            color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.white
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
            color: ttkTheme.white

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
                            height: dpHeight(70)
                            color: ttkTheme.white

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
                                color: ttkTheme.alphaLv9
                            }

                            Rectangle {
                                width: dpWidth(5)
                                height: parent.height*2/3
                                anchors {
                                    top: parent.top
                                    topMargin: parent.height/3/2
                                }
                                color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.white
                            }

                            Text {
                                id: titleAreaSong
                                text: title
                                width: ttkOnlineSearchPage.width - iconAreaSong.width - dpHeight(120)
                                anchors {
                                    top: parent.top
                                    topMargin: dpHeight(10)
                                    left: parent.left
                                    leftMargin: dpHeight(20)
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
                                    topMargin: dpHeight(5)
                                    left: parent.left
                                    leftMargin: dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_normal"
                            }

                            Text {
                                text: artist
                                width: titleAreaSong.width - iconAreaSong.width
                                anchors {
                                    top: titleAreaSong.bottom
                                    topMargin: dpHeight(10)
                                    left: iconAreaSong.right
                                    leftMargin: dpHeight(10)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height/4
                                color: ttkTheme.gray
                            }
                        }
                    }

                    model: ListModel {
                        id: searedSongListModel
                    }

                    Component.onCompleted: {
                        currentIndex = -1;
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
                            height: dpHeight(70)
                            color: ttkTheme.white

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
                                color: ttkTheme.alphaLv9
                            }

                            Rectangle {
                                width: dpWidth(5)
                                height: parent.height*2/3
                                anchors {
                                    top: parent.top
                                    topMargin: parent.height/3/2
                                }
                                color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.white
                            }

                            Text {
                                id: titleAreaMV
                                text: title
                                width: ttkOnlineSearchPage.width - iconAreaMV.width - dpHeight(120)
                                anchors {
                                    top: parent.top
                                    topMargin: dpHeight(10)
                                    left: parent.left
                                    leftMargin: dpHeight(20)
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
                                    topMargin: dpHeight(5)
                                    left: parent.left
                                    leftMargin: dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_normal"
                            }

                            Text {
                                text: artist
                                width: titleAreaMV.width - iconAreaMV.width
                                anchors {
                                    top: titleAreaMV.bottom
                                    topMargin: dpHeight(10)
                                    left: iconAreaMV.right
                                    leftMargin: dpHeight(10)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height/4
                                color: ttkTheme.gray
                            }
                        }
                    }

                    model: ListModel {
                        id: searedMVListModel
                    }

                    Component.onCompleted: {
                        currentIndex = -1;
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
                            height: dpHeight(70)
                            color: ttkTheme.white

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    searedLrcList.currentIndex = index;
                                    TTK_NETWORK.setCurrentIndex(index);
                                }
                            }

                            Rectangle {
                                width: ttkMainWindow.width
                                height: 1
                                color: ttkTheme.alphaLv9
                            }

                            Rectangle {
                                width: dpWidth(5)
                                height: parent.height*2/3
                                anchors {
                                    top: parent.top
                                    topMargin: parent.height/3/2
                                }
                                color: parent.ListView.isCurrentItem ? ttkTheme.topbar_background : ttkTheme.white
                            }

                            Text {
                                id: titleAreaLrc
                                text: title
                                width: ttkOnlineSearchPage.width - iconAreaLrc.width - dpHeight(120)
                                anchors {
                                    top: parent.top
                                    topMargin: dpHeight(10)
                                    left: parent.left
                                    leftMargin: dpHeight(20)
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
                                    topMargin: dpHeight(5)
                                    left: parent.left
                                    leftMargin: dpHeight(20)
                                }
                                source: "qrc:/image/ic_playlist_normal"
                            }

                            Text {
                                text: artist
                                width: titleAreaLrc.width - iconAreaLrc.width
                                anchors {
                                    top: titleAreaLrc.bottom
                                    topMargin: dpHeight(10)
                                    left: iconAreaLrc.right
                                    leftMargin: dpHeight(10)
                                }
                                elide: Text.ElideRight
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height/4
                                color: ttkTheme.gray
                            }
                        }
                    }

                    model: ListModel {
                        id: searedLrcListModel
                    }

                    Component.onCompleted: {
                        currentIndex = -1;
                    }
                }

            }
        }
    }
}
