/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import "Core"

Rectangle {
    id: ttkMusicCleanCachedPage
    width: parent.width
    height: parent.height

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
                        ttkOutStackView.pop();
                    }
                }

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: ttkTheme.color_white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: mainMenubar.height/2
                    text: qsTr("清理占用空间")
                }
            }
        }

        ///function bar
        Rectangle {
            id: functionBar
            Layout.fillWidth: true
            height: ttkMusicCleanCachedPage.height - mainMenubar.height
            color: ttkTheme.color_white

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                Rectangle {
                    Layout.fillWidth: true
                    height: functionBar.height/2
                    color: ttkTheme.topbar_background

                    Image {
                        anchors.centerIn: parent
                        height: parent.height*2/3
                        width: height
                        source: "qrc:/image/cached_scan_down_normal"

                        ColumnLayout {
                            spacing: 0
                            anchors.fill: parent

                            Text {
                                anchors {
                                    horizontalCenter: parent.horizontalCenter
                                    bottom: cachedSizeArea.top
                                }
                                color: ttkTheme.color_white
                                text: qsTr("QQ音乐缓存")
                            }

                            Text {
                                id: cachedSizeArea
                                anchors.centerIn: parent
                                color: ttkTheme.color_white
                                font.pixelSize: parent.height/7
                                text: "666.4MB"
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: functionBar.height/2
                    color: ttkTheme.color_white

                    ColumnLayout {
                        spacing: 0
                        anchors.fill: parent

                        TTKTextButton {
                            Layout.alignment: Qt.AlignCenter
                            width: ttkGlobal.dpWidth(140)
                            height: width/2
                            textColor: ttkTheme.topbar_background
                            text: qsTr("清理缓存")
                            radius: 10
                            border {
                                color: ttkTheme.topbar_background
                                width: 1
                            }
                        }
                    }
                }
            }
        }
    }
}
