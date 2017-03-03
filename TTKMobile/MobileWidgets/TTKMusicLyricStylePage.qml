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

Rectangle {
    id: ttkMusicLyricStylePage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    Connections {
        target: TTK_LRC
        onCurrentParameterIndexChanged: {
            sizeListView.currentIndex = sIndex;
            colorListView.currentIndex = cIndex;
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicLyricStylePage.visible = false;
        }
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

            RowLayout {
                Layout.preferredHeight: ttkGlobal.dpHeight(50)
                Layout.fillWidth: true

                Text {
                    id: sizeLabelArea
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: ttkGlobal.dpWidth(100)
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    color: ttkTheme.color_gray
                    text: qsTr("大小")
                }

                ListView {
                    id: sizeListView
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: ttkMusicLyricStylePage.width - sizeLabelArea.width
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    orientation: ListView.Horizontal
                    spacing: ttkGlobal.dpHeight(5)
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        width: ttkGlobal.dpWidth(50)
                        height: ttkGlobal.dpHeight(50)
                        color: ttkTheme.color_alpha_lv8

                        Text {
                            width: parent.width
                            height: parent.height
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignHCenter
                            text: "A"
                            font.pixelSize: ttkGlobal.dpHeight( itemSize )
                            color: parent.ListView.isCurrentItem ? ttkTheme.color_green : ttkTheme.color_black
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                switch(index) {
                                    case 0:
                                    case 1:
                                    case 2:
                                    case 3:
                                    case 4:
                                    case 5:
                                        sizeListView.currentIndex = index;
                                        TTK_LRC.setCenterSize(index, itemSize);
                                        break;
                                    default: break;
                                }
                            }
                        }
                    }

                    model: ListModel {
                        ListElement {
                            itemSize: 20
                        }
                        ListElement {
                            itemSize: 23
                        }
                        ListElement {
                            itemSize: 26
                        }
                        ListElement {
                            itemSize: 29
                        }
                        ListElement {
                            itemSize: 32
                        }
                    }
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 1
                color: ttkTheme.color_alpha_lv10
            }

            RowLayout {
                Layout.preferredHeight: ttkGlobal.dpHeight(50)

                Text {
                    id: colorLabelArea
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: ttkGlobal.dpWidth(100)
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    color: ttkTheme.color_gray
                    text: qsTr("颜色")
                }

                ListView {
                    id: colorListView
                    anchors.verticalCenter: parent.verticalCenter
                    Layout.preferredWidth: ttkMusicLyricStylePage.width - colorLabelArea.width
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    orientation: ListView.Horizontal
                    spacing: ttkGlobal.dpHeight(17)
                    clip: true
                    boundsBehavior: Flickable.StopAtBounds

                    delegate: Rectangle {
                        id: wrapper
                        width: ttkGlobal.dpWidth(40)
                        height: ttkGlobal.dpHeight(40)
                        radius: ttkGlobal.dpHeight(20)

                        Rectangle {
                            width: parent.width
                            height: parent.height
                            color: itemColor
                            radius: parent.radius

                            Image {
                                anchors.fill: parent
                                source: wrapper.ListView.isCurrentItem ? "qrc:/image/action_icon_submit" : ""
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                    switch(index) {
                                    case 0:
                                    case 1:
                                    case 2:
                                    case 3:
                                    case 4:
                                    case 5:
                                        colorListView.currentIndex = index;
                                        TTK_LRC.setCenterColor(index, itemColor);
                                        break;
                                    default: break;
                                }
                            }
                        }
                    }

                    model: ListModel {
                        ListElement {
                            itemColor: "#00FF00"
                        }
                        ListElement {
                            itemColor: "#0000FF"
                        }
                        ListElement {
                            itemColor: "#FFFF00"
                        }
                        ListElement {
                            itemColor: "#FF00FF"
                        }
                        ListElement {
                            itemColor: "#FF0000"
                        }
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
                font.pixelSize: ttkMusicLyricStylePage.height/25
                text: "完成"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMusicLyricStylePage.visible = false;
                    }
                }
            }
        }
    }

    NumberAnimation {
        id: verticalYAnimation
        property: "y"
        target: ttkMusicLyricStylePage
        from: ttkMusicLyricStylePage.height
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
        var docRoot = ttkMusicLyricStylePage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicLyricStylePage.parent = docRoot;
    }
}
