/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import TTKRadioHelper 1.0
import "Core"

Item {
    id: ttkMainPage
    width: parent.width
    height: parent.height

    function updateItemShowCount() {
        funcLocalMusic.subTitle = TTK_APP.mediaCount(ttkTheme.music_normal_list);
        funcRecent.subTitle = TTK_APP.mediaCount(ttkTheme.music_recent_list);
    }

    Connections {
        target: TTK_APP
        onImportSongFinished: {
            if(index === ttkTheme.music_normal_list) {
                funcLocalMusic.subTitle = TTK_APP.mediaCount(ttkTheme.music_normal_list);
            }
        }
        onCurrentIndexChanged: {
            updateItemShowCount();
        }
        onUpdateItemShowCount: {
            updateItemShowCount();
        }
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: ttkMainMenubar
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.topbar_background

            RowLayout {
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/top_tab_more_selected"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkMainStackView.push("qrc:/MobileWidgets/TTKMainSettingPage.qml");
                    }
                }

                TTKImageButton {
                    source: "qrc:/image/top_tab_search_selected"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.right: parent.right
                    onPressed: {
                        ttkMainStackView.push("qrc:/MobileWidgets/TTKOnlineSearchPage.qml");
                    }
                }

                Rectangle {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                }
            }
        }

        ///main body
        ScrollView {
            Layout.preferredWidth: ttkMainWindow.width
            Layout.preferredHeight: ttkMainStackView.height
            horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff

            Rectangle {
                anchors {
                    right: parent.right
                    top: parent.top
                }
                width: ttkMainWindow.width
                height: ttkGlobal.dpHeight(800)
                color: "#EEEEEE"

                ColumnLayout {
                    spacing: 5
                    anchors.fill: parent

                    ///userWidget
                    Rectangle {
                        id: userWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(80)
                        anchors.left: parent.left
                        color: ttkTheme.color_white

                        TTKRadiusImage {
                            id: userImage
                            width: parent.height
                            height: parent.height
                            anchors {
                                left: parent.left
                                top: parent.top
                                leftMargin: ttkGlobal.dpHeight(10)
                            }
                            color: ttkTheme.color_alpha_lv0
                            foreground: "qrc:/image/main_background"
                            background: "qrc:/image/radius_mask"
                        }

                        Text {
                            id: userMainTitle
                            anchors {
                                left: userImage.right
                                top: parent.top
                                leftMargin: ttkGlobal.dpHeight(10)
                                topMargin: ttkGlobal.dpHeight(10)
                            }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: parent.height/4
                            text: qsTr("天天酷音")
                        }

                        Text {
                            id: userSubTitle
                            anchors {
                                left: userImage.right
                                top: userMainTitle.bottom
                                leftMargin: ttkGlobal.dpHeight(10)
                                topMargin: ttkGlobal.dpHeight(10)
                            }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: ttkTheme.color_gray
                            text: qsTr("畅想无限音乐")
                        }
                    }

                    ///functionWidget
                    Rectangle {
                        id: functionWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(310)
                        anchors.left: parent.left
                        color: ttkTheme.color_white

                        TTKMainFunctionItem {
                            id: funcLocalMusic
                            anchors {
                                left: parent.left
                                top: parent.top
                            }
                            source: "qrc:/image/mymusic_icon_allsongs_highlight"
                            mainTitle: qsTr("本地歌曲")
                            subTitle: TTK_APP.mediaCount(ttkTheme.music_normal_list)
                            onPressed: {
                                TTK_APP.setToolBoxIndex(ttkTheme.music_normal_list);
                                ttkMainStackView.push("qrc:/MobileWidgets/TTKMusicListsPage.qml");
                            }
                        }

                        TTKMainFunctionItem {
                            id: funcDownload
                            anchors {
                                left: funcLocalMusic.right
                                top: parent.top
                            }
                            source: "qrc:/image/mymusic_icon_download_normal"
                            mainTitle: qsTr("下载歌曲")
                            onPressed: {
                                TTK_APP.setToolBoxIndex(ttkTheme.music_download_list);
                                ttkMainStackView.push("qrc:/MobileWidgets/TTKMusicDownloadListsPage.qml");
                            }
                        }

                        TTKMainFunctionItem {
                            id: funcRecent
                            anchors {
                                left: funcDownload.right
                                top: parent.top
                            }
                            source: "qrc:/image/mymusic_icon_history_highlight"
                            mainTitle: qsTr("最近播放")
                            subTitle: TTK_APP.mediaCount(ttkTheme.music_recent_list)
                            onPressed: {
                                TTK_APP.setToolBoxIndex(ttkTheme.music_recent_list);
                                ttkMainStackView.push("qrc:/MobileWidgets/TTKMusicRecentListsPage.qml");
                            }
                        }

                        TTKMainFunctionItem {
                            id: funcMyLove
                            anchors {
                                left: parent.left
                                top: funcLocalMusic.bottom
                            }
                            source: "qrc:/image/mymusic_icon_favorite_normal"
                            mainTitle: qsTr("我喜欢")
                            onPressed: {
                                TTK_APP.setToolBoxIndex(ttkTheme.music_lovest_list);
                                ttkMainStackView.push("qrc:/MobileWidgets/TTKMusicLovestListsPage.qml");
                            }
                        }

                        TTKMainFunctionItem {
                            id: funcMusicVideo
                            anchors {
                                left: funcMyLove.right
                                top: funcLocalMusic.bottom
                            }
                            source: "qrc:/image/mymusic_icon_mv_highlight"
                            mainTitle: qsTr("下载MV")
                            onPressed: {
                                TTK_APP.setToolBoxIndex(ttkTheme.music_downmv_list);
                                ttkMainStackView.push("qrc:/MobileWidgets/TTKMVDownloadListsPage.qml");
                            }
                        }

                        TTKMainFunctionItem {
                            id: funcIdentifySong
                            anchors {
                                left: funcMusicVideo.right
                                top: funcLocalMusic.bottom
                            }
                            source: "qrc:/image/mymusic_icon_recognizer_normal"
                            mainTitle: qsTr("听歌识曲")
                            subTitle: qsTr("[绿旋风]")
                            onPressed: {
                                TTK_APP.setToolBoxIndex(ttkTheme.music_musicrg_list);
                                ttkMainStackView.push("qrc:/MobileWidgets/TTKMusicIdentifyListsPage.qml");
                            }
                        }
                    }

                    ///radioWidget
                    Rectangle {
                        id: radioWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(70)
                        anchors.left: parent.left
                        color: ttkTheme.color_white

                        TTKRadioHelper {
                            id: ttkRaioHelper
                            onPicDownloadFinished: {
                                radioImageArea.source = path;
                            }
                        }

                        Rectangle {
                            id: radioImage
                            width: parent.height
                            height: parent.height
                            anchors {
                                left: parent.left
                                top: parent.top
                                leftMargin: ttkGlobal.dpHeight(10)
                            }
                            color: ttkTheme.color_alpha_lv0

                            Image {
                                id: radioImageArea
                                anchors.fill: parent
                                source: "qrc:/image/default_background"
                            }

                            TTKImageButton {
                                anchors.fill: parent
                                source: "qrc:/image/radio_play_play"
                                onPressed: {
                                    ttkRaioHelper.init();
                                    if(ttkRaioHelper.isPlaying()) {
                                        source = "qrc:/image/radio_play_play";
                                        ttkRaioHelper.pause();
                                    }else {
                                        source = "qrc:/image/radio_play_pause";
                                        ttkRaioHelper.play();
                                    }
                                }
                            }
                        }

                        Text {
                            id: radioMainTitle
                            anchors {
                                left: radioImage.right
                                top: parent.top
                                leftMargin: ttkGlobal.dpHeight(10)
                                topMargin: ttkGlobal.dpHeight(10)
                            }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: parent.height/4
                            text: qsTr("个性电台")
                        }

                        Text {
                            id: radioSubTitle
                            anchors {
                                left: radioImage.right
                                top: radioMainTitle.bottom
                                leftMargin: ttkGlobal.dpHeight(10)
                                topMargin: ttkGlobal.dpHeight(10)
                            }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: ttkTheme.color_gray
                            text: qsTr("偶遇身边好音乐")
                        }
                    }

                    ///songsWidget
                    Rectangle {
                        id: songsWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(300)

                        ColumnLayout {
                            spacing: 0
                            anchors.fill: parent

                            Rectangle {
                                Layout.fillWidth: true
                                height: ttkGlobal.dpHeight(50)
                                Layout.alignment: Qt.AlignCenter

                                Text {
                                    text: qsTr("我 的 歌 单")
                                    horizontalAlignment: Qt.AlignHCenter
                                    verticalAlignment: Qt.AlignVCenter
                                    width: songsWidget.width
                                    height: parent.height
                                }

                                Image {
                                    id: image
                                    anchors.right: parent.right
                                    width: parent.height
                                    height: parent.height
                                    source: "qrc:/image/ic_toolbar_advance"
                                }
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                color: ttkTheme.color_alpha_lv10
                                height: 1
                            }

                            Rectangle {
                                Layout.fillWidth: true
                                Layout.preferredHeight: ttkGlobal.dpHeight(150)

                                Rectangle {
                                    id: songsItemWidget
                                    height: ttkGlobal.dpHeight(70)
                                    width: ttkGlobal.dpWidth(70)
                                    color: ttkTheme.color_alpha_lv11
                                    anchors {
                                        left: parent.left
                                        top: parent.top
                                        leftMargin: ttkGlobal.dpHeight(10)
                                        topMargin: ttkGlobal.dpHeight(10)
                                    }

                                    Image {
                                        id: songsItemImage
                                        anchors.fill: parent
                                        source: "qrc:/image/ic_add_folder"
                                    }

                                    Text {
                                        text: qsTr("新建歌单")
                                        anchors {
                                            left: songsItemImage.right
                                            top: parent.top
                                            leftMargin: ttkGlobal.dpHeight(10)
                                        }
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        width: parent.width
                                        height: parent.height
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
