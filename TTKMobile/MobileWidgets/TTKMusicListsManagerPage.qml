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

Item {
    id: ttkMusicListsManagerPage
    width: parent.width
    height: parent.height

    property bool selectAll: true

    function removeItemFromList() {
        var array = getSelectedIndex();
        if(array.length === 0) {
            ttkFlyInOutBox.start();
        }

        for(var i=array.length-1; i>=0; --i) {
            var index = array[i];
            playlistModel.remove(index);
            TTK_APP.removeMusicSongsFromManager(ttkGlobal.list_module_index, index);
        }
    }

    function getSelectedIndex() {
        var array = new Array;
        for(var i=0; i<playlistModel.count; ++i) {
            if(playlistModel.get(i).checkState === true)
            {
                array.push(i);
            }
        }
        return array;
    }

    function updateModel(state) {
        playlistModel.clear();
        var names = TTK_APP.mediaNames(ttkGlobal.list_module_index);
        var artists = TTK_APP.mediaArtists(ttkGlobal.list_module_index);
        for(var i=0; i<names.length; ++i) {
            var info = {
                checkState: state,
                title: names[i],
                artist: artists[i]
            }
            playlistModel.append(info);
        }
    }

    Component.onCompleted: {
        updateModel(false);
    }

    Rectangle {
        anchors.fill: parent
        color: ttkTheme.color_white
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
                spacing: 2
                anchors.fill: parent

                TTKTextButton {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    textColor: ttkTheme.color_white
                    text: qsTr("全选")
                    onClicked: {
                        updateModel(selectAll);
                        selectAll = !selectAll;
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
                    text: qsTr("批量编辑")
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
            id: mainBody
            Layout.fillWidth: true
            height: ttkMusicListsManagerPage.height - mainMenubar.height - bottomArea.height
            color: ttkTheme.color_white

            ListView {
                id: itemListView
                anchors.fill: parent
                clip: true

                delegate: Component {
                    Rectangle {
                        id: wrapper
                        width: ttkMainWindow.width
                        height: ttkGlobal.dpHeight(70)
                        color: ttkTheme.color_white

                        Rectangle {
                            width: ttkMainWindow.width
                            height: 1
                            color: ttkTheme.color_alpha_lv9
                        }

                        CheckBox {
                            id: selectCheckbox
                            anchors {
                                top: parent.top
                                topMargin: ttkGlobal.dpHeight(15)
                                left: parent.left
                                leftMargin: ttkGlobal.dpWidth(10)
                            }
                            checked: checkState
                            width: parent.height
                            style: CheckBoxStyle {
                                indicator: Image {
                                    width: ttkGlobal.dpWidth(40)
                                    height: ttkGlobal.dpHeight(40)
                                    source: control.checked ? "qrc:/image/ic_lyric_poster_lyric_select" :
                                                              "qrc:/image/ic_lyric_poster_gray_unselect"
                                }
                            }
                        }

                        Text {
                            id: titleArea
                            text: title
                            width: ttkMusicListsManagerPage.width - iconArea.width - ttkGlobal.dpWidth(60)
                            anchors {
                                top: parent.top
                                topMargin: ttkGlobal.dpHeight(10)
                                left: selectCheckbox.right
                            }
                            elide: Text.ElideRight
                            verticalAlignment: Qt.AlignVCenter
                            font.pixelSize: parent.height*3/10
                        }

                        Image {
                            id: iconArea
                            width: parent.height/3
                            height: parent.height/3
                            anchors {
                                top: titleArea.bottom
                                topMargin: ttkGlobal.dpHeight(5)
                                left: selectCheckbox.right
                            }
                            source: "qrc:/image/ic_playlist_normal"
                        }

                        Text {
                            id: artistArea
                            text: artist
                            width: titleArea.width - iconArea.width
                            anchors {
                                top: titleArea.bottom
                                topMargin: ttkGlobal.dpHeight(10)
                                left: iconArea.right
                                leftMargin: ttkGlobal.dpHeight(10)
                            }
                            elide: Text.ElideRight
                            verticalAlignment: Qt.AlignVCenter
                            font.pixelSize: parent.height/4
                            color: ttkTheme.color_gray
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                selectCheckbox.checked = playlistModel.get(index).checkState ? false : true;
                                playlistModel.set(index, {
                                    checkState: selectCheckbox.checked
                                });
                            }
                        }
                    }
                }

                model: ListModel {
                    id: playlistModel
                }
            }
        }

        ///bottom area
        Rectangle {
            id: bottomArea
            anchors {
                left: parent.left
                leftMargin: ttkGlobal.dpWidth(20)
            }
            width: ttkMusicListsManagerPage.width
            height: ttkGlobal.dpHeight(120)
            color: ttkTheme.color_white

            RowLayout {
                spacing: 0
                anchors.fill: parent

                TTKImageTextButton {
                    anchors {
                        top: parent.top
                        topMargin: ttkGlobal.dpHeight(10)
                    }
                    Layout.alignment: Qt.AlignTop
                    source: "qrc:/image/action_delete_normal"
                    text: qsTr("删除")
                    onClicked: {
                        var array = getSelectedIndex();
                        if(array.length === 0) {
                            ttkFlyInOutBox.color = ttkTheme.color_red
                            ttkFlyInOutBox.start();
                            return;
                        }

                        ttkMusicDeletePage.visible = true;
                    }
                }

                TTKImageTextButton {
                    anchors {
                        top: parent.top
                        topMargin: ttkGlobal.dpHeight(10)
                    }
                    Layout.alignment: Qt.AlignTop
                    source: "qrc:/image/action_download_normal"
                    text: qsTr("下载")
                    onClicked: {
                        var array = getSelectedIndex();
                        if(array.length === 0) {
                            ttkFlyInOutBox.color = ttkTheme.color_red
                            ttkFlyInOutBox.start();
                            return;
                        }

                        ttkMusicSongDownloadPage.visible = false;
                        ttkFlyInOutBox.color = ttkTheme.topbar_background;
                        for(var i=0; i<array.length; ++i) {
                            var modelItem = playlistModel.get(array[i]);
                            ttkMusicSongDownloadPage.text = modelItem.artist + " - " + modelItem.title;
                            ttkMusicSongDownloadPage.visible = true;
                        }
                    }
                }

                TTKImageTextButton {
                    anchors {
                        top: parent.top
                        topMargin: ttkGlobal.dpHeight(10)
                    }
                    Layout.alignment: Qt.AlignTop
                    source: "qrc:/image/action_add_to_list_normal"
                    text: qsTr("添加到")
                    onClicked: {
                        var array = getSelectedIndex();
                        if(array.length === 0) {
                            ttkFlyInOutBox.color = ttkTheme.color_red
                            ttkFlyInOutBox.start();
                        }
                    }
                }

                TTKImageTextButton {
                    anchors {
                        top: parent.top
                        topMargin: ttkGlobal.dpHeight(10)
                    }
                    Layout.alignment: Qt.AlignTop
                    source: "qrc:/image/action_icon_qzone"
                    text: qsTr("背景音乐")
                    onClicked: {
                        var array = getSelectedIndex();
                        if(array.length === 0) {
                            ttkFlyInOutBox.color = ttkTheme.color_red
                            ttkFlyInOutBox.start();
                        }
                    }
                }
            }
        }
    }

    TTKMusicSongDownloadPage {
        id: ttkMusicSongDownloadPage
    }

    TTKMusicDeletePage {
        id: ttkMusicDeletePage
    }

    TTKFlyInOutBox {
        id: ttkFlyInOutBox
        text: qsTr("请选择需要操作的歌曲记录");
    }
}
