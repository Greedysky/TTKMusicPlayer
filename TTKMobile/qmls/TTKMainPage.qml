import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Item {
    id: ttkMainPage
    width: parent.width
    height: parent.height

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: ttkMainMenubar
            width: ttkMainWindow.width
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.topbar_background

            RowLayout {
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKButton {
                    source: "qrc:/image/top_tab_more_selected"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        console.log("asdasd");
                    }
                }

                TTKButton {
                    source: "qrc:/image/top_tab_search_selected"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.right: parent.right
                    onPressed: {
                        ttkMainStackView.push("qrc:/qmls/TTKOnlineSearchPage.qml");
                    }
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
                height: dpHeight(700)
                color: "#EEEEEE"

                ColumnLayout {
                    spacing: 5
                    anchors.fill: parent

                    ///userWidget
                    Rectangle {
                        id: userWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(80)
                        anchors.left: parent.left
                        color: ttkTheme.white

                        Rectangle {
                            id: userImage
                            width: parent.height
                            height: parent.height
                            anchors {
                                left: parent.left
                                top: parent.top
                                leftMargin: dpHeight(10)
                            }
                            color: ttkTheme.alphaLv0

                            Image {
                                anchors.fill: parent
                                source: "qrc:/image/test"
                            }
                        }

                        Text {
                            id: userMainTitle
                            anchors {
                                left: userImage.right
                                top: parent.top
                                leftMargin: dpHeight(10)
                                topMargin: dpHeight(10)
                            }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            font.pixelSize: 15
                            text: "个性电台"
                        }

                        Text {
                            id: userSubTitle
                            anchors {
                                left: userImage.right
                                top: userMainTitle.bottom
                                leftMargin: dpHeight(10)
                                topMargin: dpHeight(10)
                            }
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: "gray"
                            text: "偶遇身边好音乐"
                        }
                    }

                    ///functionWidget
                    Rectangle {
                        id: functionWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(310)
                        anchors.left: parent.left
                        color: ttkTheme.white

                        TTKMainFunctionItem {
                            id: funcLocalMusic
                            anchors {
                                left: parent.left
                                top: parent.top
                            }
                            source: "qrc:/image/mymusic_icon_allsongs_highlight"
                            mainTitle: "本地歌曲"
                            subTitle: "749"
                        }

                        TTKMainFunctionItem {
                            id: funcDownload
                            anchors {
                                left: funcLocalMusic.right
                                top: parent.top
                            }
                            source: "qrc:/image/mymusic_icon_download_normal"
                            mainTitle: "下载歌曲"
                        }

                        TTKMainFunctionItem {
                            id: func_recent
                            anchors {
                                left: funcDownload.right
                                top: parent.top
                            }
                            source: "qrc:/image/mymusic_icon_history_highlight"
                            mainTitle: "最近播放"
                            subTitle: "20"
                        }

                        TTKMainFunctionItem {
                            id: funcMyLove
                            anchors {
                                left: parent.left
                                top: funcLocalMusic.bottom
                            }
                            source: "qrc:/image/mymusic_icon_favorite_normal"
                            mainTitle: "我喜欢"
                        }

                        TTKMainFunctionItem {
                            id: funcMusicVideo
                            anchors {
                                left: funcMyLove.right
                                top: funcLocalMusic.bottom
                            }
                            source: "qrc:/image/mymusic_icon_mv_highlight"
                            mainTitle: "下载MV"
                        }

                        TTKMainFunctionItem {
                            id: funcIdentifySong
                            anchors {
                                left: funcMusicVideo.right
                                top: funcLocalMusic.bottom
                            }
                            source: "qrc:/image/mymusic_icon_recognizer_normal"
                            mainTitle: "听歌识曲"
                            subTitle: "[绿旋风]"
                        }
                    }

                    ///radioWidget
                    Rectangle {
                        id: radioWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(70)
                        anchors.left: parent.left
                        color: ttkTheme.white

                        Rectangle {
                            id: radioImage
                            width: parent.height
                            height: parent.height
                            anchors {
                                left: parent.left
                                top: parent.top
                                leftMargin: dpHeight(10)
                            }
                            color: ttkTheme.alphaLv0

                            Image {
                                anchors.fill: parent
                                source: "qrc:/image/test"
                            }
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
                            font.pixelSize: 15
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
                    }

                    ///songsWidget
                    Rectangle {
                        id: songsWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(200)
                        anchors.left: parent.left
                        color: "yellow"
                    }
                }
            }
        }
    }
}
