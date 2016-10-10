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
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.alphaLv0

            RowLayout {
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
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
            color: ttkTheme.alphaLv0

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                Image {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredHeight: dpHeight(25)
                    Layout.preferredWidth: dpWidth(104)
                    source: "qrc:/image/dts_big_logo"
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: dpHeight(51)
                    color: "gray"

                    RowLayout {
                        anchors.fill: parent
                        Text {
                            text: qsTr("DTS 音效")
                            color: ttkTheme.alphaLv6
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(15)
                            }
                        }

                        TTKImageButton {
                            id: dtsEffectButton
                            anchors {
                                right: parent.right
                                rightMargin: dpWidth(15)
                            }
                            Layout.preferredHeight: dpHeight(29)
                            Layout.preferredWidth: dpWidth(65)
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
                    Layout.preferredHeight: dpHeight(200)
                    color: ttkTheme.alphaLv0

                    ColumnLayout {
                        spacing: 0
                        anchors.fill: parent

                        Text {
                            text: qsTr("选择音效模式")
                            color: ttkTheme.alphaLv4
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(15)
                            }
                        }

                        ListView {
                            id: itemListView
                            anchors {
                                left: parent.left
                                leftMargin: dpHeight(25)
                            }
                            Layout.fillWidth: true
                            Layout.preferredHeight: dpHeight(150)
                            orientation: ListView.Horizontal
                            boundsBehavior: Flickable.StopAtBounds
                            clip: true
                            spacing: dpHeight(3)

                            delegate: Rectangle {
                                id: wrapper1
                                width: dpWidth(120)
                                height: dpHeight(120)
                                color: ttkTheme.alphaLv0

                                Rectangle {
                                    width: parent.width*0.9
                                    height: parent.height*0.9
                                    color: ttkTheme.alphaLv0
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
                                            color: ttkTheme.alphaLv6
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
                    Layout.preferredHeight: dpHeight(51)
                    color: "gray"

                    RowLayout {
                        anchors.fill: parent
                        Text {
                            text: qsTr("均衡器")
                            color: ttkTheme.alphaLv6
                            anchors {
                                left: parent.left
                                leftMargin: dpWidth(15)
                            }
                        }

                        TTKImageButton {
                            anchors {
                                right: parent.right
                                rightMargin: dpWidth(15)
                            }
                            Layout.preferredHeight: dpHeight(29)
                            Layout.preferredWidth: dpWidth(29)
                            source: "qrc:/image/ic_toolbar_advance"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            ttkOutStackView.push("qrc:/qmls/TTKMusicEqualizerPage.qml");
                            TTK_PLAYER.setMusicEnhanced(0);
                            itemListView.currentIndex = -1;
                            dtsEffectButton.source = "qrc:/image/dts_switch_off";
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: dpHeight(200)
                    color: ttkTheme.alphaLv0
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
