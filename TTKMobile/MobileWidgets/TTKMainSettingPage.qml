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
import "Core"

Item {
    id: ttkMainSettingPage
    width: parent.width
    height: parent.height

    function timeToQuitApp(time) {
        firstListModel.set(3, {
            imageSource: "qrc:/image/more_icon_timer",
            imageSubSource: "qrc:/image/switch_on_normal",
            title: qsTr("定时关闭")
        });
        var beforeTime = Date.parse(new Date);
        var afterTime = new Date(beforeTime + time*1000*60);
        ttkFlyInOutBox.text = qsTr("设置成功，") +
                              TTK_UTILS.fromMSecsSinceEpoch(afterTime*1, "hh:mm") +
                              qsTr("后退出");
        ttkFlyInOutBox.start();
        TTK_APP.setTimeToQuitApp(time*1000*60);
    }

    function updateWifiConnect(state) {
        var justUseWifi = TTK_UTILS.getValue("MobileWifiConnectChoiced");
        if(state)
        {
            justUseWifi = !justUseWifi;
            TTK_UTILS.setValue("MobileWifiConnectChoiced", justUseWifi);
        }
        TTK_UTILS.setNetworkBlockNotWifi();

        firstListModel.set(1, {
            imageSource: "qrc:/image/more_icon_wifionly",
            imageSubSource: justUseWifi ? "qrc:/image/switch_on_normal"
                                        : "qrc:/image/switching_off",
            title: qsTr("仅Wi-Fi联网")
        });
    }

    Component.onCompleted: {
        updateWifiConnect(false);
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
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
                    anchors.left: parent.left
                    onClicked: {
                        ttkMainStackView.pop();
                    }
                }

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: ttkTheme.color_white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: mainMenubar.height*2/5
                    text: qsTr("更多")
                }

                Rectangle {
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                }
            }
        }

        ///main body
        ScrollView {
            Layout.preferredWidth: ttkMainWindow.width
            Layout.preferredHeight: ttkMainStackView.height
            horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
            verticalScrollBarPolicy: Qt.ScrollBarAlwaysOff

            Rectangle {
                anchors {
                    right: parent.right
                    top: parent.top
                }
                width: ttkMainWindow.width
                height: ttkGlobal.dpHeight(1000)
                color: "#EEEEEE"

                ColumnLayout {
                    spacing: 5
                    anchors.fill: parent

                    Rectangle {
                        id: functionWidget
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(150)
                        anchors.left: parent.left
                        color: ttkTheme.color_white

                        TTKMainFunctionItem {
                            id: vipItemArea
                            height: (parent.height-10)
                            anchors {
                                left: parent.left
                                top: parent.top
                            }
                            source: "qrc:/image/more_icon_myvip_normal"
                            mainTitle: qsTr("升级为VIP")
                            subTitle: qsTr("畅想音乐特权")
                        }

                        TTKMainFunctionItem {
                            id: themeItemArea
                            height: (parent.height-10)
                            anchors {
                                left: vipItemArea.right
                                top: parent.top
                            }
                            source: "qrc:/image/more_icon_personal_center"
                            mainTitle: qsTr("个性化主题")
                            subTitle: qsTr("默认主题")
                        }

                        TTKMainFunctionItem {
                            id: msgItemArea
                            height: (parent.height-10)
                            anchors {
                                left: themeItemArea.right
                                top: parent.top
                            }
                            source: "qrc:/image/more_icon_notificationcenter"
                            mainTitle: qsTr("消息中心")
                        }
                    }

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(243)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: ttkGlobal.dpHeight(60)
                            color: ttkTheme.color_white

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    switch(index) {
                                        case 0: break;
                                    }
                                }
                            }

                            TTKImageFunctionItem {
                                source: imageSource
                                subSource: imageSubSource
                                text: title
                                textColor: ttkTheme.color_black
                                onImageButtonClicked: {
                                    switch(index) {
                                        case 0:
                                            ttkOutStackView.push("qrc:/MobileWidgets/TTKMainMoreSettingPage.qml");
                                            break;
                                        case 1:
                                            updateWifiConnect(true);
                                            break;
                                        case 2: break;
                                        case 3:
                                            if(TTK_APP.timeToQuitAppIsSet()) {
                                                firstListModel.set(3, {
                                                    imageSource: "qrc:/image/more_icon_timer",
                                                    imageSubSource: "qrc:/image/switching_off",
                                                    title: qsTr("定时关闭")
                                                });
                                                TTK_APP.setTimeToQuitApp(-1);
                                            }else {
                                                ttkMusicTimeSettingPage.visible = true;
                                            }
                                            break;
                                    }
                                }
                            }
                        }

                        model: ListModel{
                            id: firstListModel
                            ListElement {
                                imageSource: "qrc:/image/more_icon_settings"
                                imageSubSource: "qrc:/image/ic_toolbar_advance"
                                title: qsTr("设置")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_wifionly"
                                imageSubSource: "qrc:/image/switching_off"
                                title: qsTr("仅Wi-Fi联网")
                            }
                            ListElement {
                                imageSource: "qrc:/image/simple_mode_icon"
                                imageSubSource: "qrc:/image/switching_off"
                                title: qsTr("简洁模式")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_timer"
                                imageSubSource: "qrc:/image/switching_off";
                                title: qsTr("定时关闭")
                            }
                        }

                        Component.onCompleted: {
                            if(TTK_APP.timeToQuitAppIsSet()) {
                                firstListModel.set(3, {
                                    imageSource: "qrc:/image/more_icon_timer",
                                    imageSubSource: "qrc:/image/switch_on_normal",
                                    title: qsTr("定时关闭")
                                });
                            }
                        }
                    }

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(304)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: ttkGlobal.dpHeight(60)
                            color: ttkTheme.color_white

                            TTKImageFunctionItem {
                                source: imageSource
                                text: title
                                textColor: ttkTheme.color_black
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    switch(index) {
                                        case 0: break;
                                        case 1:
                                            if(ttkGlobal.isAndroid()) {
                                                ttkOutStackView.push("qrc:/MobileWidgets/QRCodeReader.qml");
                                            }
                                            break;
                                        case 2: break;
                                        case 3:
                                            ttkOutStackView.push("qrc:/MobileWidgets/TTKMusicCleanCachedPage.qml");
                                            break;
                                        case 4: break;
                                    }
                                }
                            }
                        }

                        model: ListModel{
                            ListElement {
                                imageSource: "qrc:/image/more_icon_chinaunicom"
                                title: qsTr("免流量服务")
                            }
                            ListElement {
                                imageSource: "qrc:/image/pc2device_icon"
                                title: qsTr("传歌到手机")
                            }
                            ListElement {
                                imageSource: "qrc:/image/player_btn_qplayon_normal"
                                title: qsTr("QPlay与车载音乐")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_cleancache"
                                title: qsTr("清理占用空间")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_wifionly"
                                title: qsTr("免费WIFI")
                            }
                        }
                    }

                    ListView {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(121)
                        boundsBehavior: Flickable.StopAtBounds
                        clip: true
                        spacing: 1

                        delegate: Rectangle {
                            width: ttkMainWindow.width
                            height: ttkGlobal.dpHeight(60)
                            color: ttkTheme.color_white

                            TTKImageFunctionItem {
                                source: imageSource
                                text: title
                                textColor: ttkTheme.color_black
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    switch(index) {
                                        case 0:
                                            TTK_UTILS.updateApplicationDialog();
                                            break;
                                        case 1:
                                            ttkMusicAboutPage.visible = true;
                                            break;
                                    }
                                }
                            }
                        }

                        model: ListModel{
                            ListElement {
                                imageSource: "qrc:/image/more_icon_about"
                                title: qsTr("软件更新")
                            }
                            ListElement {
                                imageSource: "qrc:/image/more_icon_about"
                                title: qsTr("关于天天酷音")
                            }
                        }
                    }

                    TTKTextButton {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(60)
                        color: ttkTheme.color_white
                        textColor: ttkTheme.color_red
                        text: qsTr("退出")
                        onClicked: {
                            Qt.quit();
                        }
                    }

                    Rectangle {
                        Layout.preferredWidth: ttkMainWindow.width
                        Layout.preferredHeight: ttkGlobal.dpHeight(60)
                        color: ttkTheme.color_alpha_lv0
                    }
                }
            }
        }
    }

    TTKMusicTimeSettingPage {
        id: ttkMusicTimeSettingPage
    }

    TTKMusicAboutPage {
        id: ttkMusicAboutPage
    }

    TTKFlyInOutBox {
        id: ttkFlyInOutBox
    }
}
