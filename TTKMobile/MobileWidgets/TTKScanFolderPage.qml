/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import Qt.labs.folderlistmodel 2.1
import "Core"

Item {
    id: ttkScanFolderPage
    width: parent.width
    height: parent.height

    property variant scanPaths: []
    signal pathChanged(variant scanPath)

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

                Rectangle {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                }

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: ttkTheme.color_white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: mainMenubar.height*2/5
                    text: qsTr("扫描指定的文件夹")
                }

                TTKTextButton {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.right: parent.right
                    textColor: ttkTheme.color_white
                    text: qsTr("关闭")

                    onPressed: {
                        ttkOutStackView.pop();
                    }
                }
            }
        }

        ///function area
        Rectangle {
            id: functionArea
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(60)
            color: ttkTheme.color_white

            RowLayout {
                spacing: 0
                anchors.fill: parent

                TTKImageButton {
                    id: goBackButton
                    Layout.preferredWidth: ttkGlobal.dpWidth(40)
                    Layout.preferredHeight: ttkGlobal.dpHeight(40)
                    anchors {
                        left: parent.left
                        leftMargin: ttkGlobal.dpHeight(5)
                    }
                    source: folderModel.parentFolder !== "" ? "qrc:/image/scanning_icon_up"
                                                            : "qrc:/image/scanning_icon_up_disable"
                    onPressed: {
                        if(folderModel.parentFolder !== "") {
                            folderModel.folder = folderModel.parentFolder;
                        }
                    }
                }

                Text {
                    id: filePathArea
                    text: folderModel.folder;
                    Layout.preferredWidth: ttkScanFolderPage.width - ttkGlobal.dpWidth(80)
                    Layout.fillHeight: true
                    anchors {
                        left: goBackButton.right
                        leftMargin: ttkGlobal.dpWidth(5)
                    }
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: functionArea.height/3
                    elide: Text.ElideLeft
                    color: ttkTheme.color_gray
                }
            }
        }

        ///main body
        Rectangle {
            id: mainBody
            Layout.fillWidth: true
            height: ttkScanFolderPage.height - mainMenubar.height - functionArea.height - bottomArea.height
            color: ttkTheme.color_alpha_lv9

            ListView {
                id: listView
                anchors.fill: parent
                clip: true

                FolderListModel {
                    id: folderModel
                    showFiles: false
                    showHidden: true
                    folder: TTK_UTILS.getRootPath()
                }

                model: folderModel
                delegate: Rectangle {
                    id: wrapper
                    width: listView.width
                    height: ttkGlobal.dpHeight(60)
                    color: ttkTheme.color_alpha_lv0

                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        CheckBox {
                            id: checkBoxArea
                            Layout.preferredHeight: ttkGlobal.dpHeight(30)
                            Layout.alignment: Qt.AlignCenter
                            anchors {
                                left: parent.left
                                leftMargin: ttkGlobal.dpHeight(10)
                            }
                            style: CheckBoxStyle {
                                indicator: Image {
                                    width: ttkGlobal.dpWidth(30)
                                    height: ttkGlobal.dpHeight(30)
                                    source: control.checked ? "qrc:/image/ic_lyric_poster_lyric_select"
                                                            : "qrc:/image/ic_lyric_poster_lyric_unselect"
                                }
                            }
                            onClicked: {
                                if(checkBoxArea.checked) {
                                    scanPaths.push(fileURL);
                                }else {
                                    scanPaths.pop(fileURL);
                                }
                            }
                        }

                        Image {
                            id: iconArea
                            Layout.preferredWidth: ttkGlobal.dpWidth(40)
                            Layout.preferredHeight: ttkGlobal.dpHeight(40)
                            Layout.alignment: Qt.AlignCenter
                            anchors {
                                left: checkBoxArea.right
                                leftMargin: ttkGlobal.dpHeight(10)
                            }
                            source: "qrc:/image/custom_file_type_dir"
                        }

                        Text {
                            id: titleArea
                            text: fileName
                            Layout.preferredWidth: ttkScanFolderPage.width - ttkGlobal.dpHeight(140)
                            Layout.fillHeight: true
                            anchors {
                                left: iconArea.right
                                leftMargin: ttkGlobal.dpWidth(10)
                            }
                            verticalAlignment: Qt.AlignVCenter
                            font.pixelSize: wrapper.height/3
                            elide: Text.ElideRight

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    scanPaths = [];
                                    listView.currentIndex = index;
                                    folderModel.folder = folderModel.get(index, "fileURL");
                                }
                            }
                        }
                    }

                    Rectangle {
                        width: ttkScanFolderPage.width
                        height: 1
                        anchors {
                            left: parent.left
                            leftMargin: checkBoxArea.width + ttkGlobal.dpWidth(15)
                            bottom: parent.bottom
                        }
                        color: ttkTheme.color_alpha_lv9
                    }
                }
            }
        }

        ///bottom area
        Rectangle {
            id: bottomArea
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(60)
            color: ttkTheme.color_white

            TTKTextButton {
                anchors.centerIn: parent
                width: ttkGlobal.dpWidth(180)
                height: ttkGlobal.dpHeight(40)
                textColor: ttkTheme.color_white
                color: ttkTheme.topbar_background
                radius: 10
                text: qsTr("开始扫描")

                onPressed: {
                    if(scanPaths.length !== 0) {
                        ttkScanFolderPage.pathChanged(scanPaths);
                    }else {
                        ttkFlyInOutBox.color = ttkTheme.color_red
                        ttkFlyInOutBox.text = qsTr("请选择扫描的文件夹")
                        ttkFlyInOutBox.start();
                    }
                }
            }
        }
    }

    TTKFlyInOutBox {
        id: ttkFlyInOutBox
    }
}
