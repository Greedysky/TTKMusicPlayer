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

Item {
    id: ttkMainMoreSettingPage
    width: parent.width
    height: parent.height

    Component.onCompleted: {
        itemListView.currentIndex = TTK_NETWORK.getCurrentServer();
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
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.left: parent.left
                    onClicked: {
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
                    font.pixelSize: mainMenubar.height*2/5
                    text: qsTr("设置")
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
            height: ttkMainMoreSettingPage.height - mainMenubar.height
            color: ttkTheme.color_white

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                Text {
                    id: infomationLabel
                    anchors {
                        left: parent.left
                        leftMargin: ttkGlobal.dpWidth(10)
                    }
                    Layout.fillWidth: true
                    Layout.preferredHeight: ttkGlobal.dpHeight(60)
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: mainMenubar.height*2/5
                    text: qsTr("数据服务器选择:")
                }

                ListView {
                    id: itemListView
                    Layout.fillWidth: true
                    Layout.preferredHeight: ttkGlobal.dpHeight(60)*6
                    spacing: 0
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        width: itemListView.width
                        height: ttkGlobal.dpHeight(60)
                        Image {
                            id: imageArea
                            source: serverImage
                            width: ttkGlobal.dpWidth(40)
                            height: ttkGlobal.dpHeight(40)
                            anchors {
                                left: parent.left
                                leftMargin: ttkGlobal.dpWidth(10)
                                top: parent.top
                                topMargin: ttkGlobal.dpHeight(10)
                            }
                        }
                        Text {
                            anchors {
                                left: imageArea.right
                                leftMargin: ttkGlobal.dpWidth(10)
                                top: parent.top
                                topMargin: ttkGlobal.dpHeight(20)
                            }
                            width: parent.width
                            text: title
                        }

                        Rectangle {
                            width: parent.width
                            height: 1
                            color: ttkTheme.color_alpha_lv9
                        }

                        Image {
                            source: parent.ListView.isCurrentItem ? "qrc:/image/ic_playlist_normal" : ""
                            width: ttkGlobal.dpWidth(30)
                            height: ttkGlobal.dpHeight(30)
                            anchors {
                                right: parent.right
                                rightMargin: ttkGlobal.dpHeight(20)
                                top: parent.top
                                topMargin: ttkGlobal.dpHeight(15)
                                bottom: parent.bottom
                                bottomMargin: ttkGlobal.dpHeight(15)
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                ttkFlyInOutBox.start();
                                itemListView.currentIndex = index;
                                TTK_NETWORK.setCurrentServer(index);
                            }
                        }
                    }

                    model: ListModel {
                        id: itemListModel
                        ListElement {
                            imageSource: "qrc:/image/more_icon_settings"
                            serverImage: "qrc:/server/lb_wangyiyun"
                            title: qsTr("网易云音乐")
                        }
                        ListElement {
                            imageSource: "qrc:/image/more_icon_settings"
                            serverImage: "qrc:/server/lb_qq"
                            title: qsTr("QQ音乐")
                        }
                        ListElement {
                            imageSource: "qrc:/image/more_icon_settings"
                            serverImage: "qrc:/server/lb_xiami"
                            title: qsTr("虾米音乐")
                        }
                        ListElement {
                            imageSource: "qrc:/image/more_icon_settings"
                            serverImage: "qrc:/server/lb_kuwo"
                            title: qsTr("酷我音乐")
                        }
                        ListElement {
                            imageSource: "qrc:/image/more_icon_settings"
                            serverImage: "qrc:/server/lb_kugou"
                            title: qsTr("酷狗音乐")
                        }
                        ListElement {
                            imageSource: "qrc:/image/more_icon_settings"
                            serverImage: "qrc:/server/lb_baidu"
                            title: qsTr("百度音乐")
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: ttkGlobal.dpHeight(60)*5
                }
            }
        }

        TTKFlyInOutBox {
            id: ttkFlyInOutBox
            text: qsTr("设置成功");
        }
    }
}
