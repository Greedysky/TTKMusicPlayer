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
    id: ttkMainPage
    width: parent.width
    height: parent.height

    Connections {
        target: TTK_NETWORK
        onNetworkConnectionStateChanged: {
            networkWidget.visible = !state;
        }
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: ttkMainMenubar
            Layout.fillWidth: true
            Layout.preferredHeight: ttkGlobal.dpHeight(ttkTheme.topbar_height)
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
                    onClicked: {
                        ttkMainStackView.push("qrc:/MobileWidgets/TTKMainSettingPage.qml");
                    }
                }

                TTKImageButton {
                    source: "qrc:/image/top_tab_search_selected"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.right: parent.right
                    onClicked: {
                        ttkMainStackView.push("qrc:/MobileWidgets/TTKMusicOnlineSearchPage.qml");
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
                height: ttkGlobal.dpHeight(500)
                color: "#EEEEEE"

                ColumnLayout {
                    spacing: 5
                    anchors.fill: parent

                    ///networkWidget
                    Rectangle {
                        id: networkWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(30)
                        anchors.left: parent.left
                        color: ttkTheme.color_white
                        visible: false

                        Text {
                            anchors.fill: parent
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignHCenter
                            text: qsTr("你已经进入了一个无网络的异次元空间")
                            color: ttkTheme.color_red
                        }
                    }

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

                    ///imageWidget
                    Rectangle {
                        id: functionWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(310)
                        anchors.left: parent.left
                        color: ttkTheme.color_white

                        RowLayout {
                            spacing: 2
                            anchors.fill: parent

                            Image {
                                Layout.alignment: Qt.AlignCenter
                                source: "qrc:/image/default_background"
                            }
                        }
                    }

                    ///textWidget
                    Rectangle {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(80)
                        anchors.left: parent.left
                        color: ttkTheme.color_white

                        RowLayout {
                            spacing: 2
                            anchors.fill: parent

                            Rectangle {
                                Layout.preferredWidth: ttkGlobal.dpWidth(50)
                            }

                            Text {
                                id: serverText
                                Layout.alignment: Qt.AlignCenter
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                horizontalAlignment: Qt.AlignHCenter
                                verticalAlignment: Qt.AlignVCenter
                                font.pixelSize: parent.height/4
                                text: TTK_APP.getCurrentServerName()
                            }

                            Rectangle {
                                Layout.preferredWidth: ttkGlobal.dpWidth(50)
                            }
                        }
                    }

                }
            }
        }
    }

    onVisibleChanged: {
        serverText.text = TTK_APP.getCurrentServerName();
    }
}
