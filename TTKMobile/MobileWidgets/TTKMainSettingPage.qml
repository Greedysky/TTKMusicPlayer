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
    id: ttkMainSettingPage
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
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.left: parent.left
                    onClicked: {
                        ttkMainStackView.pop();
                    }
                }

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: ttkTheme.color_white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: mainMenubar.height*2/5
                    text: qsTr("更多")
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

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(243)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: ttkGlobal.dpHeight(60)
                            color: ttkTheme.color_white

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    switch(index) {
                                        case 0: break;
                                    }
                                }
                            }

                            TTKImageFunctionItem {
                                source: imageSource
                                subSource: imageSubSource
                                text: title
                                textColor: ttkTheme.color_black
                                onImageButtonClicked: {
                                    switch(index) {
                                        case 0:
                                            ttkOutStackView.push("qrc:/MobileWidgets/TTKMainMoreSettingPage.qml");
                                            break;
                                        case 1:
                                            TTK_UTILS.updateApplicationDialog();
                                            break;
                                        case 2:
                                            ttkMusicAboutPage.visible = true;
                                            break;
                                        default: break;
                                    }
                                }
                            }
                        }

                        model: ListModel{
                            id: firstListModel
                            ListElement {
                                imageSource: "qrc:/image/more_icon_settings"
                                imageSubSource: "qrc:/image/ic_toolbar_advance"
                                title: qsTr("设置")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_about"
                                title: qsTr("软件更新")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_about"
                                title: qsTr("关于天天酷音")
                            }
                        }
                    }

                    TTKTextButton {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(60)
                        color: ttkTheme.color_white
                        textColor: ttkTheme.color_red
                        text: qsTr("退出")
                        onClicked: {
                            Qt.quit();
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(360)
                        color: "#EEEEEE"
                    }
                }
            }
        }
    }

    TTKMusicAboutPage {
        id: ttkMusicAboutPage
    }

    TTKFlyInOutBox {
        id: ttkFlyInOutBox
    }
}
