import QtQuick 2.4
import QtGraphicalEffects 1.0
import QtQuick.Controls 1.2

Rectangle {
    id: ttkMusicLyricPage

    property alias m_currentIndex: lyricView.currentIndex

    ListView {
        id: lyricView
        anchors.fill: parent
        highlightRangeMode: ListView.StrictlyEnforceRange
        clip: true
        spacing: 15

        model: TTK_LRC

        highlight: Rectangle {
            color: "#00000000"
            Behavior on y {
                SmoothedAnimation {
                    duration: 300
                }
            }
        }

        delegate: Rectangle {
            id: lyricDelegateId
            width: parent.width
            height: 20
            color: "#00000000"

            Text {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                text: textLine
                color: parent.ListView.isCurrentItem ? "#00ff00" : "#4C4C4C"
                font.pixelSize: parent.ListView.isCurrentItem ? dpHeight(30) : dpHeight(26)
                font.bold: parent.ListView.isCurrentItem
                font.family: qsTr("楷体")
            }

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.OpenHandCursor
            }
        }
    }

//    OpacityMask {
//        id: lyricMask
//        visible: ttkMusicLyricPage.visible
//        anchors.fill: lyricView
//        source: lyricView
//        maskSource: Rectangle {
//            anchors.top: parent.top
//            anchors.horizontalCenter: parent.horizontalCenter
//            color: "#ff0000"
//        }
//    }
}
