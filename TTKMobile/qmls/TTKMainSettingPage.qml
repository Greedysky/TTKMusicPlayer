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
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.topbar_background

            RowLayout {
                id: mainMenubarLayout
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

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: ttkTheme.white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: mainMenubar.height*2/5
                    text: qsTr("更多")
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
                height: dpHeight(1200)
                color: "#EEEEEE"

                ColumnLayout {
                    spacing: 5
                    anchors.fill: parent

                    Rectangle {
                        id: functionWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(150)
                        anchors.left: parent.left
                        color: ttkTheme.white

                        TTKMainFunctionItem {
                            id: vipItemArea
                            height: (parent.height-10)
                            anchors {
                                left: parent.left
                                top: parent.top
                            }
                            source: "qrc:/image/more_icon_myvip_normal"
                            mainTitle: qsTr("升级为VIP")
                            subTitle: qsTr("畅想音乐特权")
                        }

                        TTKMainFunctionItem {
                            id: themeItemArea
                            height: (parent.height-10)
                            anchors {
                                left: vipItemArea.right
                                top: parent.top
                            }
                            source: "qrc:/image/more_icon_personal_center"
                            mainTitle: qsTr("个性化主题")
                            subTitle: qsTr("默认主题")
                        }

                        TTKMainFunctionItem {
                            id: msgItemArea
                            height: (parent.height-10)
                            anchors {
                                left: themeItemArea.right
                                top: parent.top
                            }
                            source: "qrc:/image/more_icon_notificationcenter"
                            mainTitle: qsTr("消息中心")
                        }
                    }

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(243)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: dpHeight(60)
                            color: ttkTheme.white

                            TTKImageFunctionItem {
                                source: imageSource
                                subSource: imageSubSource
                                text: title
                                textColor: ttkTheme.black
                            }
                        }

                        model: ListModel{
                            ListElement {
                                imageSource: "qrc:/image/more_icon_settings"
                                title: qsTr("设置")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_wifionly"
                                imageSubSource: "qrc:/image/switching_off"
                                title: qsTr("仅Wi-Fi联网")
                            }
                            ListElement {
                                imageSource: "qrc:/image/simple_mode_icon"
                                imageSubSource: "qrc:/image/switching_off"
                                title: qsTr("简洁模式")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_timer"
                                imageSubSource: "qrc:/image/switching_off"
                                title: qsTr("定时关闭")
                            }
                        }
                    }

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(304)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: dpHeight(60)
                            color: ttkTheme.white

                            TTKImageFunctionItem {
                                source: imageSource
                                text: title
                                textColor: ttkTheme.black
                            }
                        }

                        model: ListModel{
                            ListElement {
                                imageSource: "qrc:/image/more_icon_chinaunicom"
                                title: qsTr("免流量服务")
                            }
                            ListElement {
                                imageSource: "qrc:/image/pc2device_icon"
                                title: qsTr("传歌到手机")
                            }
                            ListElement {
                                imageSource: "qrc:/image/player_btn_qplayon_normal"
                                title: qsTr("QPlay与车载音乐")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_cleancache"
                                title: qsTr("清理占用空间")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_wifionly"
                                title: qsTr("免费WIFI")
                            }
                        }
                    }

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(243)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: dpHeight(60)
                            color: ttkTheme.white

                            TTKImageFunctionItem {
                                source: imageSource
                                text: title
                                textColor: ttkTheme.black
                            }
                        }

                        model: ListModel{
                            ListElement {
                                imageSource: "qrc:/image/more_icon_settings"
                                title: qsTr("设置")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_wifionly"
                                subSource: "qrc:/image/switching_off"
                                title: qsTr("仅Wi-Fi联网")
                            }
                            ListElement {
                                imageSource: "qrc:/image/simple_mode_icon"
                                subSource: "qrc:/image/switching_off"
                                title: qsTr("简洁模式")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_timer"
                                subSource: "qrc:/image/switching_off"
                                title: qsTr("定时关闭")
                            }
                        }
                    }

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(61)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: dpHeight(60)
                            color: ttkTheme.white

                            TTKImageFunctionItem {
                                source: imageSource
                                text: title
                                textColor: ttkTheme.black
                            }
                        }

                        model: ListModel{
                            ListElement {
                                imageSource: "qrc:/image/more_icon_about"
                                title: qsTr("关于天天酷音")
                            }
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(60)
                        color: ttkTheme.white

                        Text {
                            anchors.fill: parent
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignHCenter
                            color: "red"
                            text: qsTr("退出")
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: dpHeight(60)
                        color: ttkTheme.alphaLv0
                    }
                }
            }
        }
    }
}
