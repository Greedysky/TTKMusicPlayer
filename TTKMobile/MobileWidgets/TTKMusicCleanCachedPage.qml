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
import TTKFileSearchCore 1.0
import "Core"

Rectangle {
    id: ttkMusicCleanCachedPage
    width: parent.width
    height: parent.height

    Component.onCompleted: {
        scanFileSizeImage.source = "qrc:/image/cached_scan_normal";
        scanFileSizeImageAnimation.start();
        searchCore.filesSize(TTK_UTILS.getCachedPath());
    }

    Connections {
        target: searchCore
        onSizeFinished: {
            scanFileSizeImageAnimation.stop();
            scanFileSizeImage.source = "qrc:/image/cached_scan_down_normal";
            cachedSizeArea.text = TTK_UTILS.size2Label(size);
        }
    }

    TTKFileSearchCore {
        id: searchCore
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
                spacing: 0
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

                Rectangle {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
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

                    Rectangle {
                        anchors.centerIn: parent
                        width: height
                        height: parent.height*2/3
                        color: ttkTheme.color_alpha_lv0

                        Image {
                            id: scanFileSizeImage
                            height: parent.height
                            width: parent.width
                            anchors.centerIn: parent
                            source: "qrc:/image/cached_scan_normal";
                        }

                        Rectangle {
                            height: parent.height
                            width: parent.width
                            anchors.centerIn: parent
                            color: ttkTheme.color_alpha_lv0

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
                                }
                            }
                        }
                    }

                    RotationAnimation {
                        id: scanFileSizeImageAnimation
                        target: scanFileSizeImage
                        property: "rotation"
                        from: 0
                        to: 360
                        direction: RotationAnimation.Clockwise
                        duration: 3000
                        loops: Animation.Infinite
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
                            width: ttkGlobal.dpWidth(120)
                            height: width/2
                            textColor: ttkTheme.topbar_background
                            text: qsTr("清理缓存")
                            radius: 10
                            border {
                                color: ttkTheme.topbar_background
                                width: 1
                            }
                            onPressed: {
                                if( TTK_UTILS.removeDir(TTK_UTILS.getCachedPath()) ) {
                                    ttkFlyInOutBox.color = ttkTheme.topbar_background
                                    ttkFlyInOutBox.text = qsTr("清理成功！");
                                }else {
                                    ttkFlyInOutBox.color = ttkTheme.color_red
                                    ttkFlyInOutBox.text = qsTr("清理失败！");
                                }
                                ttkFlyInOutBox.start();
                            }
                        }
                    }
                }
            }
        }
    }

    TTKFlyInOutBox {
        id: ttkFlyInOutBox
    }
}
