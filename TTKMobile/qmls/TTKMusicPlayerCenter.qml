import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Item {
    id: ttkMusicPlayerCenter
    width: parent.width
    height: parent.height

    Rectangle {
        anchors.fill: parent
        Image {
            id: image
            anchors.fill: parent
            source: "qrc:/image/test"
        }
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: mainMenubar
            width: ttkMusicPlayerCenter.width
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.alphaLv14

            RowLayout {
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                TTKButton {
                    source: "qrc:/image/player_btn_close_normal"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkOutStackView.pop();
                    }
                }

                TTKButton {
                    source: "qrc:/image/player_btn_more_normal"
                    Layout.preferredWidth: dpWidth(50*1.36)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.right: parent.right
                    onPressed: {

                    }
                }
            }
        }

        ///main body
        StackView {
            id: ttkMainStackView
            width: ttkMusicPlayerCenter.width
            height: ttkMusicPlayerCenter.height - playerComponent.height - mainMenubar.height
            focus: true

            initialItem: Rectangle {
                id: musicAlbum
                anchors {
                    right: ttkMainWindow.right
                    top: ttkMainWindow.top
                }
                width: ttkMusicPlayerCenter.width
                height: dpHeight(120)
                color: "yellow"
            }
        }

        ///player component
        Rectangle {
            id: playerComponent
            anchors {
                right: ttkMainWindow.right
                top: ttkMainWindow.top
            }
            width: ttkMusicPlayerCenter.width
            height: dpHeight(300)
            color: "red"

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                ///timer component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(50)
                    color: "#884569"

                    RowLayout {
                        spacing: 2
                        anchors.fill: parent

                        Text {
                            text: "00:00"
                        }

                        Slider {
                            Layout.fillWidth: true
                            height: 30
                            stepSize: 0.01
                            value: 1
                        }

                        Text {
                            text: "00:00"
                        }
                    }
                }


                ///control component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(100)
                    color: "#888888"

                    RowLayout {
                        spacing: 2
                        anchors.fill: parent

                        TTKButton {
                            source: "qrc:/image/player_btn_pre_normal"
                            Layout.preferredWidth: dpWidth(50)
                            Layout.preferredHeight: dpHeight(50)
                            anchors.left: parent.left
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_play_normal"
                            Layout.preferredWidth: dpWidth(50)
                            Layout.preferredHeight: dpHeight(50)
                            anchors.left: parent.left
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_next_normal"
                            Layout.preferredWidth: dpWidth(50)
                            Layout.preferredHeight: dpHeight(50)
                            anchors.left: parent.left
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }
                    }
                }



            }
        }
    }
}
