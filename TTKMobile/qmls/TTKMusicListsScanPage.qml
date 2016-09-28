import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Item {
    id: ttkMusicListsScanPage
    width: parent.width
    height: parent.height

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

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: ttkTheme.white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: "本地歌曲"
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

        ///main body
        Rectangle {
            Layout.fillWidth: true
            height: ttkMusicListsScanPage.height - mainMenubar.height - bottomBody.height
            color: "gray"

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                ///main body
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: dpHeight(250)
                    color: "red"
                }

                Text {
                    Layout.alignment: Qt.AlignCenter
                    text: "一键扫描手机内的歌曲文件"
                }

                RadioButton {
                    Layout.alignment: Qt.AlignCenter
                    text: "不扫描60s以下的歌曲 "
                }

                RadioButton {
                    Layout.alignment: Qt.AlignCenter
                    text: "不扫描100k以下的歌曲"
                }

                TTKTextButton {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: dpWidth(110)
                    Layout.preferredHeight: dpHeight(40)
                    textColor: ttkTheme.white
                    color: ttkTheme.topbar_background
                    radius: 4
                    text: "开始扫描"

                    onPressed: {
                        ttkOutStackView.pop();
                    }
                }
            }
        }

        ///bottom body
        Rectangle {
            id: bottomBody
            Layout.fillWidth: true
            height: dpHeight(80)

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
                    ttkOutStackView.pop();
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
