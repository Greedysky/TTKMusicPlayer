import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import "Core"

Rectangle{
    id: ttkMusicListsMorePage

    visible: false
    anchors.fill: parent
    color: ttkTheme.alphaLv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMusicListsMorePage.visible = false;
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
        boundsBehavior: Flickable.StopAtBounds

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
                        switch(index) {
                            case 0:
                                break;
                            case 1:
                                break;
                            case 2:
                                break;
                            case 3:
                                ttkMusicListsMorePage.visible = false;
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
                title: qsTr("按时间排序")
            }
            ListElement {
                imgSource: "qrc:/image/local_music_sorted_by_name_icon"
                title: qsTr("按名称排序")
            }
            ListElement {
                imgSource: "qrc:/image/local_music_upgrade_song_quality_icon"
                title: qsTr("升级音质")
            }
            ListElement {
                imgSource: "qrc:/image/local_music_scan_icon"
                title: qsTr("扫描音乐")
            }
            ListElement {
                imgSource: "qrc:/image/icon_download_lyric"
                title: qsTr("一键下载词图")
            }
            ListElement {
                imgSource: "qrc:/image/local_cloud_music_icon"
                title: qsTr("本地歌曲恢复助手")
            }
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMusicListsMorePage.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMusicListsMorePage.parent = docRoot;
    }
}
