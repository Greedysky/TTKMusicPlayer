/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Layouts 1.1
import "Core"

Rectangle {
    id: ttkTimeSettingPage
    visible: false
    width: parent.width
    height: parent.height
    color: ttkTheme.color_alpha_lv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkTimeSettingPage.visible = false;
        }
    }

    Rectangle {
        id: autoSelectdTimeArea
        visible: false
        width: parent.width
        height: ttkGlobal.dpHeight(60)*3
        color: "#F0F0F0"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ColumnLayout {
            spacing: 0
            anchors.fill: parent

            Text {
                Layout.alignment: Qt.AlignCenter
                height: parent.height/4
                font.pixelSize: parent.height/8
                text: qsTr("自定义时间")
            }

            TTKLineInput {
                id: timeInputArea
                Layout.alignment: Qt.AlignCenter
                width: parent.width*4/5
                height: parent.height/4
                fontSize: height/2
                validator: RegExpValidator {
                    regExp: /^[0-9]+$/
                }
            }

            TTKTextButton {
                Layout.alignment: Qt.AlignCenter
                width: parent.width*4/5
                height: parent.height/4
                color: ttkTheme.topbar_background
                text: qsTr("确定")
                radius: 10
                onPressed: {
                    timeToQuitApp(parseInt(timeInputArea.text));
                    ttkTimeSettingPage.visible = false;
                }
            }
        }
    }

    Rectangle {
        id: normalSelectdTimeArea
        width: parent.width
        height: itemListView.height
        color: "#F0F0F0"
        anchors {
            bottom: parent.bottom
            left: parent.left
        }

        ListView {
            id: itemListView
            width: parent.width
            height: ttkGlobal.dpHeight(60)*itemListView.count
            spacing: 0
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            delegate: Rectangle {
                width: itemListView.width
                height: ttkGlobal.dpHeight(60)
                Text {
                    anchors {
                        fill: parent
                        left: parent.left
                        leftMargin: ttkGlobal.dpHeight(20)
                    }
                    verticalAlignment: Qt.AlignVCenter
                    width: parent.width
                    text: title
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: ttkTheme.color_alpha_lv9
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        switch(index) {
                            case 0:
                                timeToQuitApp(15);
                                ttkTimeSettingPage.visible = false;
                                break;
                            case 1:
                                timeToQuitApp(30);
                                ttkTimeSettingPage.visible = false;
                                break;
                            case 2:
                                timeToQuitApp(60);
                                ttkTimeSettingPage.visible = false;
                                break;
                            case 3:
                                timeToQuitApp(90);
                                ttkTimeSettingPage.visible = false;
                                break;
                            case 4:
                                autoSelectdTimeArea.visible = true;
                                normalSelectdTimeArea.visible = false;
                                break;
                        }

                    }
                }
            }

            model: ListModel {
                ListElement{
                    title: qsTr("15分钟后")
                }
                ListElement{
                    title: qsTr("30分钟后")
                }
                ListElement{
                    title: qsTr("60分钟后")
                }
                ListElement{
                    title: qsTr("90分钟后")
                }
                ListElement{
                    title: qsTr("自定义")
                }
            }
        }
    }

    NumberAnimation {
        id: verticalYAnimation
        property: "y"
        target: ttkTimeSettingPage
        from: ttkTimeSettingPage.height
        to: 0
        duration: 200
    }

    onVisibleChanged: {
        normalSelectdTimeArea.visible = true;
        autoSelectdTimeArea.visible = false;
    }

    Component.onCompleted:
    {
        itemListView.currentIndex = -1;
        var docRoot = ttkTimeSettingPage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkTimeSettingPage.parent = docRoot;
    }
}
