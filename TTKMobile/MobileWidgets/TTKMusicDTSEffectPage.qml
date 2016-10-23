/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "Core"

Rectangle {
    id: ttkMusicDTSEffectPage
    height: parent.height
    width: parent.width
    color: "#262C3B"

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: mainMenubar
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.color_alpha_lv0

            RowLayout {
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
            }
        }

        ///main body
        Rectangle {
            Layout.fillWidth: true
            height: ttkMusicDTSEffectPage.height - mainMenubar.height
            color: ttkTheme.color_alpha_lv0

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                Image {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredHeight: ttkGlobal.dpHeight(25)
                    Layout.preferredWidth: ttkGlobal.dpWidth(104)
                    source: "qrc:/image/dts_big_logo"
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: ttkGlobal.dpHeight(51)
                    color: ttkTheme.color_gray

                    RowLayout {
                        anchors.fill: parent
                        Text {
                            text: qsTr("DTS 音效")
                            color: ttkTheme.color_alpha_lv6
                            anchors {
                                left: parent.left
                                leftMargin: ttkGlobal.dpWidth(15)
                            }
                        }

                        TTKImageButton {
                            id: dtsEffectButton
                            anchors {
                                right: parent.right
                                rightMargin: ttkGlobal.dpWidth(15)
                            }
                            Layout.preferredHeight: ttkGlobal.dpHeight(29)
                            Layout.preferredWidth: ttkGlobal.dpWidth(65)
                            source: TTK_PLAYER.getMusicEnhanced() === 0 ? "qrc:/image/dts_switch_off"
                                                                        : "qrc:/image/dts_switch_on"
                            onPressed: {
                                TTK_PLAYER.setMusicEnhanced(0);
                                source = "qrc:/image/dts_switch_off";
                                itemListView.currentIndex = -1;
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: ttkGlobal.dpHeight(200)
                    color: ttkTheme.color_alpha_lv0

                    ColumnLayout {
                        spacing: 0
                        anchors.fill: parent

                        Text {
                            text: qsTr("选择音效模式")
                            color: ttkTheme.color_alpha_lv4
                            anchors {
                                left: parent.left
                                leftMargin: ttkGlobal.dpWidth(15)
                            }
                        }

                        ListView {
                            id: itemListView
                            anchors {
                                left: parent.left
                                leftMargin: ttkGlobal.dpHeight(25)
                            }
                            Layout.fillWidth: true
                            Layout.preferredHeight: ttkGlobal.dpHeight(150)
                            orientation: ListView.Horizontal
                            boundsBehavior: Flickable.StopAtBounds
                            clip: true
                            spacing: ttkGlobal.dpHeight(3)

                            delegate: Rectangle {
                                id: wrapper1
                                width: ttkGlobal.dpWidth(120)
                                height: ttkGlobal.dpHeight(120)
                                color: ttkTheme.color_alpha_lv0

                                Rectangle {
                                    width: parent.width*0.9
                                    height: parent.height*0.9
                                    color: ttkTheme.color_alpha_lv0
                                    radius: 6

                                    Image {
                                        anchors.fill: parent
                                        source: wrapper1.ListView.isCurrentItem ? "qrc:/image/dts_tag_item_selected"
                                                                                : "qrc:/image/dts_tag_item_normal"
                                    }

                                    ColumnLayout {
                                        spacing: 0
                                        anchors.fill: parent

                                        Image {
                                            Layout.alignment: Qt.AlignCenter
                                            Layout.preferredHeight: wrapper1.height*0.6
                                            Layout.preferredWidth: wrapper1.width*0.6
                                            source: imgSource
                                        }

                                        Text {
                                            Layout.alignment: Qt.AlignCenter
                                            verticalAlignment: Qt.AlignTop
                                            horizontalAlignment: Qt.AlignHCenter
                                            color: ttkTheme.color_alpha_lv6
                                            width: parent.width
                                            text: title
                                        }
                                    }
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        itemListView.currentIndex = index;
                                        dtsEffectButton.source = "qrc:/image/dts_switch_on";
                                        TTK_PLAYER.setMusicEnhanced(index + 1);
                                    }
                                }
                            }

                            model: ListModel {
                                ListElement {
                                    imgSource: "qrc:/image/dts_preset_mode_near_chosen"
                                    title: qsTr("近场模式")
                                }
                                ListElement {
                                    imgSource: "qrc:/image/dts_preset_mode_wide_chosen"
                                    title: qsTr("宽广模式")
                                }
                                ListElement {
                                    imgSource: "qrc:/image/dts_preset_mode_front_chosen"
                                    title: qsTr("前置模式")
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: ttkGlobal.dpHeight(51)
                    color: ttkTheme.color_gray

                    RowLayout {
                        anchors.fill: parent
                        Text {
                            text: qsTr("均衡器")
                            color: ttkTheme.color_alpha_lv6
                            anchors {
                                left: parent.left
                                leftMargin: ttkGlobal.dpWidth(15)
                            }
                        }

                        TTKImageButton {
                            anchors {
                                right: parent.right
                                rightMargin: ttkGlobal.dpWidth(15)
                            }
                            Layout.preferredHeight: ttkGlobal.dpHeight(29)
                            Layout.preferredWidth: ttkGlobal.dpWidth(29)
                            source: "qrc:/image/ic_toolbar_advance"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            ttkOutStackView.push("qrc:/MobileWidgets/TTKMusicEqualizerPage.qml");
                            TTK_PLAYER.setMusicEnhanced(0);
                            itemListView.currentIndex = -1;
                            dtsEffectButton.source = "qrc:/image/dts_switch_off";
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: ttkGlobal.dpHeight(200)
                    color: ttkTheme.color_alpha_lv0
                }
            }
        }
    }

    Component.onCompleted: {
        var index = TTK_PLAYER.getMusicEnhanced();
        if(index === 0) {
            itemListView.currentIndex = -1;
        }else {
            itemListView.currentIndex = index - 1;
        }
    }
}
