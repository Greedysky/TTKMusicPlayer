/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Window 2.0

import "Core"

ApplicationWindow {
    id: ttkMainWindow
    visible: true
    width: Qt.platform.os === "android" ? Screen.desktopAvailableWidth : 320
    height: Qt.platform.os === "android" ? Screen.desktopAvailableHeight : 480
   
    property bool exitApp: false
    property real pixelDensity: 4.46
    property real multiplierHeight: ttkMainWindow.height/480
    property real multiplierWidth: ttkMainWindow.width/320

    function dpHeight(numbers) {
        return Math.round(numbers*((pixelDensity*25.4)/160)*multiplierHeight);
    }
    function dpWidth(numbers) {
        return Math.round(numbers*((pixelDensity*25.4)/160)*multiplierHeight);
    }

    TTKTheme {
        id: ttkTheme
    }

    StackView {
        id: ttkOutStackView
        width: parent.width
        height: parent.height
        focus: true
        initialItem: ColumnLayout {
            spacing: 0

            StackView {
                id: ttkMainStackView
                width: ttkMainWindow.width
                height: ttkMainWindow.height - ttkMusicBar.height
                focus: true
                initialItem: TTKMainPage {
                }
            }

            TTKMusicBar {
                id: ttkMusicBar
            }
        }
    }

    TTKMessageBox {
        id: ttkMessageBox
        title: qsTr("提示")
        content: qsTr("是否继续退出")
        okText: qsTr("退出")
        cancelText: qsTr("取消")
        onResult: {
            if(val === 1) {
                ttkMainWindow.exitApp = true;
                ttkMainWindow.close();
                Qt.quit();
            }
        }
    }

    onClosing: {
        if(ttkOutStackView.depth > 1) {
            ttkOutStackView.pop();
            close.accepted = false;
        }else
        {
            if(ttkMainStackView.depth > 1) {
                ttkMainStackView.pop();
                close.accepted = false;
            }
            else {
                close.accepted = ttkMainWindow.exitApp;
                if(!close.accepted) {
                    ttkMessageBox.visible = true;
                }
            }
        }
    }
}
