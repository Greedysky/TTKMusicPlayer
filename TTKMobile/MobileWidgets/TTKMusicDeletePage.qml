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
    id: ttkMusicDeletePage

    visible: false
    anchors.fill: parent
    color: ttkTheme.color_alpha_lv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicDeletePage.visible = false;
        }
    }

    Rectangle {
        id: mainRectangle
        color: ttkTheme.color_white
        radius: 10
        width: 0.7*parent.width
        height: 0.3*parent.height
        anchors.centerIn: parent

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            Text {
                anchors {
                    top: parent.top
                    topMargin: ttkGlobal.dpHeight(20)
                }
                Layout.preferredHeight: ttkGlobal.dpHeight(60)
                Layout.alignment: Qt.AlignCenter
                font.pixelSize: parent.height/9
                text: qsTr("删除歌曲")
            }

            Text {
                anchors {
                    left: parent.left
                    leftMargin: ttkGlobal.dpWidth(20)
                }
                Layout.preferredHeight: ttkGlobal.dpHeight(30)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: qsTr("确定删除选中的歌曲?")
                color: ttkTheme.color_gray
            }

            CheckBox {
                anchors {
                    left: parent.left
                    leftMargin: ttkGlobal.dpWidth(20)
                }
                Layout.preferredHeight: ttkGlobal.dpHeight(30)
                text: qsTr("同时删除本地文件")
                style: CheckBoxStyle {
                    indicator: Image {
                        width: ttkGlobal.dpWidth(20)
                        height: ttkGlobal.dpHeight(20)
                        source: control.checked ? "qrc:/image/ic_lyric_poster_lyric_select" :
                                                  "qrc:/image/ic_lyric_poster_gray_unselect"
                    }
                }
            }

            Rectangle {
                id: bottomBody
                anchors.bottom: parent.bottom
                Layout.preferredWidth: mainRectangle.width
                height: ttkGlobal.dpHeight(60)

                TTKTextButton {
                    border {
                        color: ttkTheme.color_alpha_lv9
                        width: 2
                    }
                    width: bottomBody.width/2
                    height: bottomBody.height
                    anchors.left: parent.left
                    color: ttkTheme.color_white
                    text: qsTr("取消")
                    onClicked: {
                        ttkMusicDeletePage.visible = false;
                    }
                }

                TTKTextButton {
                    border {
                        color: ttkTheme.color_alpha_lv9
                        width: 2
                    }
                    width: bottomBody.width/2
                    height: bottomBody.height
                    anchors.right: parent.right
                    color: ttkTheme.color_white
                    text: qsTr("确定")
                    onClicked: {
                        removeItemFromList();
                        ttkMusicDeletePage.visible = false;
                    }
                }
            }
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicDeletePage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicDeletePage.parent = docRoot;
    }
}
