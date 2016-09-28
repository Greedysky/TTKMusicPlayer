import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Rectangle{
    id: ttkMusicListsPageMore

    visible: false
    anchors.fill: parent
    color: ttkTheme.alphaLv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicListsPageMore.visible = false;
        }
    }

    ListView {
        id: listView
        anchors.top: parent.top
        anchors.topMargin: dpHeight(ttkTheme.topbar_height)
        anchors.right: parent.right
        width: dpWidth(240)
        height: dpHeight(60)*6
        clip: true

        delegate: Rectangle {
                id: wrapper
                width: listView.width
                height: dpHeight(60)
                color: ttkTheme.white

                Rectangle {
                    width: ttkMainWindow.width
                    height: 1
                    color: ttkTheme.alphaLv9
                }

                Image {
                    id: iconArea
                    width: wrapper.height
                    height: wrapper.height
                    anchors {
                        top: parent.top
                        left: parent.left
                        leftMargin: dpHeight(5)
                    }
                    source: imgSource
                }

                Text {
                    id: titleArea
                    text: title
                    anchors {
                        left: iconArea.right
                        leftMargin: dpHeight(5)
                    }
                    height: wrapper.height
                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter
                    font.pixelSize: height/3
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        switch(index)
                        {
                            case 0:
                                break;
                            case 1:
                                break;
                            case 2:
                                break;
                            case 3:
                                ttkMusicListsPageMore.visible = false;
                                ttkOutStackView.push("qrc:/qmls/TTKMusicListsScanPage.qml");
                                break;
                            case 4:
                                break;
                            case 5:
                                break;
                            default:
                                break;
                        }
                    }
                }
            }

        model: ListModel {
            ListElement {
                imgSource: "qrc:/image/local_music_sorted_by_time_icon"
                title: "按时间排序"
            }
            ListElement {
                imgSource: "qrc:/image/local_music_sorted_by_name_icon"
                title: "按名称排序"
            }
            ListElement {
                imgSource: "qrc:/image/local_music_upgrade_song_quality_icon"
                title: "升级音质"
            }
            ListElement {
                imgSource: "qrc:/image/local_music_scan_icon"
                title: "扫描音乐"
            }
            ListElement {
                imgSource: "qrc:/image/icon_download_lyric"
                title: "一键下载词图"
            }
            ListElement {
                imgSource: "qrc:/image/local_cloud_music_icon"
                title: "本地歌曲恢复助手"
            }
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicListsPageMore.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicListsPageMore.parent = docRoot;
    }
}
