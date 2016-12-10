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
import QtQuick.Controls.Styles 1.4
import "Core"

Rectangle{
    id: ttkMusicPlayerCenterLrcPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicPlayerCenterLrcPage.visible = false;
        }
    }

    TTKMusicLyricStylePage {
        id: ttkMusicLyricStylePage
    }

    Rectangle {
        width: parent.width
        height: parent.height*0.3
        color: "#F0F0F0"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            ListView {
                anchors {
                    left: parent.left
                    leftMargin: ttkGlobal.dpHeight(30)
                    right: parent.right
                    rightMargin: ttkGlobal.dpHeight(30)
                }
                Layout.fillWidth: true
                Layout.preferredHeight: ttkMusicPlayerCenterLrcPage.height/8
                orientation: ListView.Horizontal
                spacing: ttkGlobal.dpHeight(30)

                delegate: Rectangle {
                    id: wrapper1
                    width: ttkGlobal.dpWidth(60)
                    height: ttkGlobal.dpHeight(60)
                    radius: 10

                    Rectangle {
                        id: imageArea1
                        width: parent.width
                        height: parent.height
                        color: ttkTheme.color_white
                        radius: parent.radius

                        Image {
                            anchors.fill: parent
                            source: imgSource
                        }
                    }

                    Text {
                        anchors {
                            top: imageArea1.bottom
                            topMargin: ttkGlobal.dpHeight(20)
                        }
                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                        width: parent.width
                        text: title
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            switch(index) {
                                case 0: break;
                                case 1:
                                    ttkMusicLyricStylePage.visible = true;
                                    ttkMusicPlayerCenterLrcPage.visible = false;
                                    break;
                                case 2: break;
                                case 3: break;
                                case 4: break;
                                case 5: break;
                                case 6: break;
                            }
                        }
                    }
                }

                model: ListModel {
                    ListElement {
                        imgSource: "qrc:/image/action_lyric_poster_normal"
                        title: qsTr("歌词海报")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_lyric_fontsize_normal"
                        title: qsTr("字体样式")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_lyric_adjust_normal"
                        title: qsTr("调整进度")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_lyric_desktop_normal"
                        title: qsTr("桌面歌词")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_lyric_search_normal"
                        title: qsTr("重新搜词")
                    }
                    ListElement {
                        imgSource: "qrc:/image/action_lyric_report_normal"
                        title: qsTr("反馈错误")
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: ttkTheme.color_alpha_lv10
            }

            Text {
                Layout.alignment: Qt.AlignCenter
                verticalAlignment: Qt.AlignVCenter
                horizontalAlignment: Qt.AlignHCenter
                font.pixelSize: ttkMusicPlayerCenterLrcPage.height/25
                text: "取消"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMusicPlayerCenterLrcPage.visible = false;
                    }
                }
            }
        }
    }

    NumberAnimation {
        id: verticalYAnimation
        property: "y"
        target: ttkMusicPlayerCenterLrcPage
        from: ttkMusicPlayerCenterLrcPage.height
        to: 0
        duration: 200
    }

    onVisibleChanged: {
        if(visible === true) {
            verticalYAnimation.start();
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicPlayerCenterLrcPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicPlayerCenterLrcPage.parent = docRoot;
    }
}
