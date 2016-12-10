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
import QtGraphicalEffects 1.0

Rectangle {
    id: ttkMessageBox

    property string title
    property string content
    property alias okText: okButton.text
    property alias cancelText: cancelButton.text

    signal result(int val)

    function doModal() {
        ttkMessageBox.state = "visible";
        Qt.inputMethod.hide();
    }

    visible: false
    anchors.fill: parent
    color: ttkTheme.color_alpha_lv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMessageBox.visible = false;
            ttkMessageBox.result(0);
        }
    }

    DropShadow {
        anchors.fill: shadowRectangle
        source: shadowRectangle
        horizontalOffset: 10
        verticalOffset: 10
        radius: 10
        samples: 17
        spread: 0.0
        color: ttkTheme.color_alpha_lv13
    }

    Rectangle {
        id: shadowRectangle
        color: ttkTheme.topbar_background
        radius: 10
        width: 0.7*parent.width
        height: 0.3*parent.height
        anchors.centerIn: parent

        Rectangle {
            anchors.fill: parent
            color: ttkTheme.color_alpha_lv4
            radius: 10
        }

        Column {
            anchors.fill: parent
            Rectangle {
                radius: 10
                width: parent.width
                height: 0.25*parent.height
                color: ttkTheme.topbar_background
                Rectangle {
                    anchors.fill: parent
                    color: ttkTheme.color_alpha_lv3
                    radius: 10
                }

                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: title
                }
            }

            Text {
                id: textText
                width: parent.width
                height: 0.5*parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                text: content
                color: ttkTheme.color_black
            }
        }

        RowLayout {
            anchors.fill: parent
            Rectangle {
                width: parent.width/3
                height: 0.15*parent.height
                anchors {
                    left: parent.left
                    bottom: parent.bottom
                    leftMargin: ttkGlobal.dpWidth(width/7)
                    bottomMargin: ttkGlobal.dpWidth(height/6)
                }
                color: ttkTheme.topbar_background
                Text {
                    id: okButton
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMessageBox.visible = false;
                        ttkMessageBox.result(1);
                    }
                }
            }

            Rectangle {
                width: parent.width/3
                height: 0.15*parent.height
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    rightMargin: ttkGlobal.dpWidth(width/7)
                    bottomMargin: ttkGlobal.dpWidth(height/6)
                }
                color: ttkTheme.topbar_background
                Text {
                    id: cancelButton
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMessageBox.visible = false;
                        ttkMessageBox.result(0);
                    }
                }
            }
        }
    }


    states: State {
        name: "visible"
        PropertyChanges {
            target: ttkMessageBox;
            visible: true
        }
    }

    transitions: Transition {
        NumberAnimation {
            target: ttkMessageBox
            properties: "visible"
            duration: 250
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMessageBox.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMessageBox.parent = docRoot;
    }
}
