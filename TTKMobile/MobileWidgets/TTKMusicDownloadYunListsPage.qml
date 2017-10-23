/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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
    id: ttkMusicDownloadYunListsPage
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
                    font.pixelSize: mainMenubar.height/2
                    text: qsTr("下载历史漫游")
                }

                TTKTextButton {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.right: parent.right
                    textColor: ttkTheme.color_white
                    text: qsTr("关闭")
                    onClicked: {
                        ttkOutStackView.pop();
                    }
                }
            }
        }

        ///main body
        Rectangle {
            width: ttkMainWindow.width
            height: ttkMainWindow.height - mainMenubar.height
            color: ttkTheme.color_white

            TTKMainFunctionItem {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                source: "qrc:/image/ic_start_recognize_bottom"
                mainTitle: qsTr("空空如也")
                subTitle: qsTr("搜索更多的歌曲吧")
                mainTitleSize: ttkGlobal.dpHeight(150)/8
            }
        }
    }
}
