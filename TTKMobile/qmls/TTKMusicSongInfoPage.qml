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

Rectangle {
    id: ttkMusicSongInfoPage
    visible: false
    anchors.fill: parent
    color: ttkTheme.alphaLv12

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
        color: ttkTheme.white

        function getFixedHeight(){
            return dpHeight(mainArea.height/5.5);
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0

            TTKTextPair {
                Layout.preferredHeight: dpHeight(mainArea.height/5.5)
                anchors {
                    left: parent.left
                    leftMargin: dpWidth(20)
                }
                textFirst: qsTr("歌曲信息")
                textSecond: qsTr("已智能匹配的歌曲暂时不支持修改歌曲信息")
            }

            ScrollView {
                Layout.preferredWidth: ttkMainWindow.width
                Layout.preferredHeight: mainArea.height - dpHeight(mainArea.height/4)
                horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
                verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff

                Rectangle {
                    width: mainArea.width*0.85
                    height: 600

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        TTKTextPair {
                            id: songNameArea
                            Layout.preferredHeight: mainArea.getFixedHeight()
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(20)
                            }
                            textFirst: qsTr("歌名")
                            textSecond: ttkMusicSongSettingPage.songName
                        }

                        TTKTextPair {
                            id: singerNameArea
                            Layout.preferredHeight: mainArea.getFixedHeight()
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(20)
                            }
                            textFirst: qsTr("歌手")
                            textSecond: ttkMusicSongSettingPage.singerName
                        }

                        TTKTextPair {
                            id: albumNameArea
                            Layout.preferredHeight: mainArea.getFixedHeight()
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(20)
                            }
                            textFirst: qsTr("专辑")
                        }

                        TTKTextPair {
                            id: formatArea
                            Layout.preferredHeight: mainArea.getFixedHeight()
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(20)
                            }
                            textFirst: qsTr("格式")

                            TTKTextPair {
                                id: bitrateArea
                                anchors {
                                    fill: parent
                                    left: parent.left
                                    leftMargin: dpWidth(mainArea.width/2)
                                }
                                textFirst: qsTr("比特率")
                            }
                        }

                        TTKTextPair {
                            id: sampleArea
                            Layout.preferredHeight: mainArea.getFixedHeight()
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(20)
                            }
                            textFirst: qsTr("采样率")

                            TTKTextPair {
                                id: yearArea
                                anchors {
                                    fill: parent
                                    left: parent.left
                                    leftMargin: dpWidth(mainArea.width/2)
                                }
                                textFirst: qsTr("年份")
                            }
                        }

                        TTKTextPair {
                            id: fileSizeArea
                            Layout.preferredHeight: mainArea.getFixedHeight()
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(20)
                            }
                            textFirst: qsTr("文件大小")

                            TTKTextPair {
                                id: durationArea
                                anchors {
                                    fill: parent
                                    left: parent.left
                                    leftMargin: dpWidth(mainArea.width/2)
                                }
                                textFirst: qsTr("时长")
                            }
                        }

                        Rectangle {
                            Layout.preferredHeight: mainArea.getFixedHeight()*2
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(20)
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                spacing: 0

                                Text {
                                    id: firstTextArea
                                    color: ttkTheme.gray
                                    text: qsTr("文件路径")
                                }

                                TextArea {
                                    id: filePathArea
                                    anchors {
                                        left: parent.left
                                        leftMargin: dpWidth(-5)
                                    }
                                    textColor: ttkTheme.black
                                }
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
