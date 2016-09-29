import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

import TTKFileSearchCore 1.0
import "Core"

Item {
    id: ttkMusicListsScanPage
    width: parent.width
    height: parent.height

    property var scanFolderObj;

    TTKFileSearchCore {
        id: searchCore
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        TTKMusicListsPageMore {
            id: ttkMusicListsPageMore
        }

        ///top bar
        Rectangle {
            id: mainMenubar
            Layout.fillWidth: true
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.topbar_background

            RowLayout {
                spacing: 2
                anchors.fill: parent

                Rectangle {
                    Layout.preferredWidth: dpWidth(50)
                }

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: ttkTheme.white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: mainMenubar.height*2/5
                    text: "扫描歌曲"
                }

                TTKTextButton {
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.right: parent.right
                    textColor: ttkTheme.white
                    text: "关闭"

                    onPressed: {
                        ttkOutStackView.pop();
                    }
                }
            }
        }

        ///scan mian page
        Rectangle {
            id: scanMainPage
            Layout.fillWidth: true
            height: ttkMusicListsScanPage.height - mainMenubar.height - bottomBody.height
            color: ttkTheme.alphaLv9

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                Image {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: dpWidth(150)
                    Layout.preferredHeight: dpHeight(150)
                    source: "qrc:/image/scanning_forlder_icon"

                    Image {
                        anchors.fill: parent
                        source: "qrc:/image/scanning_animation"
                    }
                }

                Text {
                    Layout.preferredHeight: dpHeight(50)
                    Layout.alignment: Qt.AlignCenter
                    font.pixelSize: mainMenubar.height*2/5
                    font.bold: true
                    text: "一键扫描手机内的歌曲文件"
                }

                CheckBox {
                    Layout.preferredHeight: dpHeight(30)
                    Layout.alignment: Qt.AlignCenter
                    text: "不扫描060s以下的歌曲"
                    style: CheckBoxStyle {
                        indicator: Image {
                            width: dpWidth(20)
                            height: dpHeight(20)
                            source: control.checked ? "qrc:/image/ic_lyric_poster_lyric_select" :
                                                      "qrc:/image/ic_lyric_poster_lyric_unselect"
                        }
                    }
                }

                CheckBox {
                    Layout.preferredHeight: dpHeight(30)
                    Layout.alignment: Qt.AlignCenter
                    text: "不扫描100k以下的歌曲"
                    style: CheckBoxStyle {
                        indicator: Image {
                            width: dpWidth(20)
                            height: dpHeight(20)
                            source: control.checked ? "qrc:/image/ic_lyric_poster_lyric_select" :
                                                      "qrc:/image/ic_lyric_poster_lyric_unselect"
                        }
                    }
                }

                Rectangle {
                    Layout.preferredHeight: dpHeight(50)
                }

                TTKTextButton {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: dpWidth(110)
                    Layout.preferredHeight: dpHeight(40)
                    textColor: ttkTheme.white
                    color: ttkTheme.topbar_background
                    radius: 10
                    text: "开始扫描"

                    onPressed: {
                        rotationAnimation.start();
                        bottomBody.visible = false;
                        scanMainPage.visible = false;
                        bottomBodyCancel.visible = true;
                        scanningPage.visible = true;
                    }
                }

                Rectangle {
                    Layout.preferredHeight: dpHeight(20)
                }
            }
        }

        ///scanning page
        Rectangle {
            id: scanningPage
            Layout.fillWidth: true
            height: ttkMusicListsScanPage.height - mainMenubar.height - bottomBody.height
            color: ttkTheme.alphaLv9
            visible: false

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                Image {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: dpWidth(150)
                    Layout.preferredHeight: dpHeight(150)
                    source: "qrc:/image/scanning_forlder_icon"

                    Image {
                        id: scanningAnimation
                        anchors.fill: parent
                        source: "qrc:/image/scanning_animation"

                        RotationAnimation {
                            id: rotationAnimation
                            target: scanningAnimation
                            from: 0
                            to: 360
                            direction: RotationAnimation.Clockwise
                            duration: 1000
                            loops: Animation.Infinite
                        }
                    }
                }

                Rectangle {
                    Layout.preferredHeight: dpHeight(220)
                }
            }
        }

        ///bottom body cancel
        Rectangle {
            id: bottomBodyCancel
            Layout.fillWidth: true
            height: dpHeight(60)
            visible: false

            TTKTextButton {
                width: dpWidth(180)
                height: dpHeight(40)
                anchors.centerIn: parent
                textColor: ttkTheme.white
                color: ttkTheme.topbar_background
                radius: 10
                text: "取消扫描"

                onPressed: {
                    rotationAnimation.stop();
                    bottomBody.visible = true;
                    scanMainPage.visible = true;
                    bottomBodyCancel.visible = false;
                    scanningPage.visible = false;
                }
            }
        }

        ///bottom body
        Rectangle {
            id: bottomBody
            Layout.fillWidth: true
            height: dpHeight(60)

            TTKTextButton {
                width: bottomBody.width/2
                height: bottomBody.height
                anchors.left: parent.left
                color: ttkTheme.white
                text: "自定义扫描"
                border {
                    color: ttkTheme.alphaLv9
                    width: 2
                }

                onPressed: {
                    scanFolderObj = ttkOutStackView.push("qrc:/qmls/TTKScanFolderPage.qml");
                }
                Connections {
                    target: scanFolderObj
                    onPathChanged: {
                        bottomBody.visible = false;
                        scanMainPage.visible = false;
                        bottomBodyCancel.visible = true;
                        scanningPage.visible = true;
                        rotationAnimation.start();

                        ttkOutStackView.pop();
                        searchCore.search(path);
                    }
                }
                Connections {
                    target: searchCore
                    onFinished: {
                        rotationAnimation.stop();

                        bottomBody.visible = true;
                        scanMainPage.visible = true;
                        bottomBodyCancel.visible = false;
                        scanningPage.visible = false;
                    }
                }
            }

            TTKTextButton {
                width: bottomBody.width/2
                height: bottomBody.height
                anchors.right: parent.right
                color: ttkTheme.white
                text: "扫描设置"
                border {
                    color: ttkTheme.alphaLv9
                    width: 2
                }

                onPressed: {
                    ttkOutStackView.pop();
                }
            }
        }
    }
}
