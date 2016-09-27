import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

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
        TTKPageView {
            id: playCenterPageView
            Layout.fillWidth: true
            height: ttkMusicPlayerCenter.height - mainMenubar.height - playerComponent.height

            Rectangle {
                id: musicAlbumShow
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.alphaLv13
            }

            Rectangle {
                id: musicPlayerShow
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.alphaLv14
            }

            Rectangle {
                id: musicLrcShow
                width: playCenterPageView.width
                height: playCenterPageView.height
                color: ttkTheme.alphaLv15
            }
        }

        ///player component
        Rectangle {
            id: playerComponent
            Layout.fillWidth: true
            height: dpHeight(230)
            color: ttkTheme.alphaLv14

            ColumnLayout {
                spacing: 0
                anchors.fill: parent

                ///timer component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(50)
                    color: ttkTheme.alphaLv0

                    RowLayout {
                        spacing: 10
                        anchors.fill: parent

                        Rectangle {
                            Layout.preferredWidth: dpWidth(5)
                        }

                        Text {
                            text: "00:00"
                            color: ttkTheme.white
                        }

                        Slider {
                            id: musicTimeSlider
                            Layout.fillWidth: true
                            height: 30
                            stepSize: 0.01
                            value: 1
                            style: SliderStyle{
                                groove: Rectangle{
                                    implicitWidth: musicTimeSlider.width
                                    implicitHeight: dpHeight(3)
                                    color: ttkTheme.topbar_background
                                }

                                handle: Rectangle{
                                    anchors.centerIn: parent;
                                    color: ttkTheme.topbar_background
                                    width: dpWidth(20)
                                    height: dpHeight(20)
                                    radius: dpWidth(10)
                                }
                            }
                        }

                        Text {
                            text: "00:00"
                            color: ttkTheme.white
                        }

                        Rectangle {
                            Layout.preferredWidth: dpWidth(5)
                        }
                    }
                }


                ///control component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(100)
                    color: ttkTheme.alphaLv0

                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        Rectangle {
                            Layout.preferredWidth: dpWidth(20)
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_pre_normal"
                            Layout.preferredWidth: dpWidth(120)
                            Layout.preferredHeight: dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_play_normal"
                            Layout.preferredWidth: dpWidth(120)
                            Layout.preferredHeight: dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_next_normal"
                            Layout.preferredWidth: dpWidth(120)
                            Layout.preferredHeight: dpHeight(100)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        Rectangle {
                            Layout.preferredWidth: dpWidth(20)
                        }
                    }
                }

                ///other function component
                Rectangle {
                    Layout.fillWidth: true
                    height: dpHeight(80)
                    color: ttkTheme.alphaLv0

                    RowLayout {
                        spacing: 0
                        anchors.fill: parent

                        TTKButton {
                            source: "qrc:/image/player_btn_favorite_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_random_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_download_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_share_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
                            onPressed: {
                                ttkOutStackView.pop();
                            }
                        }

                        TTKButton {
                            source: "qrc:/image/player_btn_playlist_normal"
                            Layout.preferredWidth: dpWidth(70)
                            Layout.preferredHeight: dpHeight(70)
                            Layout.alignment: Qt.AlignCenter
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
