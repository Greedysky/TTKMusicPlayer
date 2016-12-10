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
import "Core"

Rectangle {
    id: ttkMusicSongInfoPage
    visible: false
    anchors.fill: parent
    color: ttkTheme.color_alpha_lv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicSongInfoPage.visible = false;
        }
    }

    Rectangle {
        id: mainArea
        width: parent.width*0.85
        height: parent.height*0.85
        anchors.centerIn: parent
        color: ttkTheme.color_white

        function getFixedHeight(){
            return ttkGlobal.dpHeight(80);
        }

        TTKTextPair {
            id: topTitleArea
            height: mainArea.getFixedHeight()
            anchors {
                top: parent.top
                topMargin: ttkGlobal.dpHeight(10)
                left: parent.left
                leftMargin: ttkGlobal.dpWidth(20)
            }
            textFirst: qsTr("歌曲信息")
            textSecond: qsTr("暂时不支持修改歌曲信息")
        }

        ScrollView {
            width: mainArea.width
            height: mainArea.height - mainArea.getFixedHeight() - ttkGlobal.dpHeight(30);
            horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            clip: true
            anchors {
                top: topTitleArea.bottom
                left: parent.left
            }

            Rectangle {
                anchors {
                    right: parent.right
                    top: parent.top
                }
                width: parent.width
                height: mainArea.getFixedHeight()*10

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    TTKTextPair {
                        id: songNameArea
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: mainArea.getFixedHeight()
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(20)
                        }
                        textFirst: qsTr("歌名")
                        textSecond: ttkMusicSongSettingPage.songName
                    }

                    TTKTextPair {
                        id: singerNameArea
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: mainArea.getFixedHeight()
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(20)
                        }
                        textFirst: qsTr("歌手")
                        textSecond: ttkMusicSongSettingPage.singerName
                    }

                    TTKTextPair {
                        id: albumNameArea
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: mainArea.getFixedHeight()
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(20)
                        }
                        textFirst: qsTr("专辑")
                    }

                    Rectangle {
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: mainArea.getFixedHeight()
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(20)
                        }

                        RowLayout {
                            spacing: 0
                            anchors.fill: parent

                            TTKTextPair {
                                id: formatArea
                                textFirst: qsTr("格式")
                            }

                            TTKTextPair {
                                id: bitrateArea
                                anchors {
                                    left: parent.right
                                    leftMargin: mainArea.width/2
                                }
                                textFirst: qsTr("比特率")
                            }
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: mainArea.getFixedHeight()
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(20)
                        }

                        RowLayout {
                            spacing: 0
                            anchors.fill: parent

                            TTKTextPair {
                                id: sampleArea
                                textFirst: qsTr("采样率")
                            }

                            TTKTextPair {
                                id: yearArea
                                anchors {
                                    left: parent.right
                                    leftMargin: mainArea.width/2
                                }
                                textFirst: qsTr("年份")
                            }
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: parent.width
                        Layout.preferredHeight: mainArea.getFixedHeight()
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(20)
                        }

                        RowLayout {
                            spacing: 0
                            anchors.fill: parent

                            TTKTextPair {
                                id: fileSizeArea
                                textFirst: qsTr("文件大小")
                            }

                            TTKTextPair {
                                id: durationArea
                                anchors {
                                    left: parent.right
                                    leftMargin: mainArea.width/2
                                }
                                textFirst: qsTr("时长")
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredWidth: parent.width - ttkGlobal.dpWidth(20)
                        Layout.preferredHeight: mainArea.getFixedHeight()*2
                        anchors {
                            left: parent.left
                            leftMargin: ttkGlobal.dpWidth(20)
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            spacing: 0

                            Text {
                                id: firstTextArea
                                color: ttkTheme.color_gray
                                text: qsTr("文件路径")
                            }

                            TextArea {
                                id: filePathArea
                                enabled: false
                                Layout.preferredWidth: mainArea.width - ttkGlobal.dpWidth(20)
                                Layout.preferredHeight: mainArea.getFixedHeight()*2
                                wrapMode: Text.WrapAnywhere
                                anchors {
                                    left: parent.left
                                    leftMargin: ttkGlobal.dpWidth(-5)
                                }
                                textColor: ttkTheme.color_black
                            }
                        }
                    }
                }
            }
        }
    }

    onVisibleChanged: {
        if(visible) {
            TTK_UTILS.readTagFromFile(ttkMusicSongSettingPage.filePath);
            albumNameArea.textSecond = TTK_UTILS.getTag(0);
            formatArea.textSecond = TTK_UTILS.getTag(1);
            bitrateArea.textSecond = TTK_UTILS.getTag(2);
            sampleArea.textSecond = TTK_UTILS.getTag(3);
            yearArea.textSecond = TTK_UTILS.getTag(4);
            fileSizeArea.textSecond = TTK_UTILS.getTag(5);
            durationArea.textSecond = TTK_UTILS.getTag(6);
            filePathArea.text = TTK_UTILS.getTag(7);
            TTK_UTILS.closeTagFromFile();
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicSongInfoPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicSongInfoPage.parent = docRoot;
    }
}
