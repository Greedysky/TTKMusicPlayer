import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Item {
    id: ttkMusicListsPage
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

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkMainStackView.pop();
                    }
                }

                Text {
                    Layout.alignment: Qt.AlignCenter
                    Layout.preferredWidth: dpWidth(mainMenubar.width)
                    Layout.fillHeight: true
                    color: ttkTheme.white
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    text: "本地歌曲"
                }

                TTKImageButton {
                    source: "qrc:/image/player_btn_more_normal"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.right: parent.right
                    onPressed: {
                        ttkMusicListsPageMore.visible = true;
                    }
                }
            }
        }

        ///main body
        Rectangle {
            width: ttkMainWindow.width
            height: ttkMainStackView.height - mainMenubar.height
            color: ttkTheme.white

            ListView {
                anchors.fill: parent
                clip: true

                delegate: Component {
                    Rectangle {
                        id: wrapper
                        width: ttkMainWindow.width
                        height: dpHeight(70)
                        color: ttkTheme.white

                        Rectangle {
                            width: ttkMainWindow.width
                            height: 1
                            color: ttkTheme.alphaLv9
                        }

                        Text {
                            id: titleArea
                            text: title
                            anchors {
                                top: parent.top
                                topMargin: dpHeight(5)
                                left: parent.left
                                leftMargin: dpHeight(20)
                            }
                            font.pixelSize: parent.height/3
                        }

                        Image {
                            id: iconArea
                            width: parent.height/3
                            height: parent.height/3
                            anchors {
                                top: titleArea.bottom
                                left: parent.left
                                leftMargin: dpHeight(20)
                            }
                            source: "qrc:/image/ic_playlist_normal"
                        }

                        TTKImageButton {
                            id: moreFuncArea
                            width: parent.height/2
                            height: parent.height/2
                            anchors {
                                top: parent.top
                                right: parent.right
                                topMargin: dpHeight(20)
                                rightMargin: dpHeight(20)
                            }
                            source: "qrc:/image/ic_playlist_more_normal"
                        }

                        Text {
                            id: artistArea
                            text: artist
                            anchors {
                                top: titleArea.bottom
                                left: iconArea.right
                                leftMargin: dpHeight(10)
                            }
                            font.pixelSize: parent.height/4
                            color: "gray"
                        }
                    }
                }

                model: ListModel {
                    id: testModel
                }
            }

            Component.onCompleted: {
                for(var i=0; i<100; ++i) {
                    testModel.append({"title": "埃文啊我", "artist": "test"});
                }
            }
        }
    }
}
