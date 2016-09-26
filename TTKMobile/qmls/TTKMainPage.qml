import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    id: ttkMainPage
    anchors.fill: parent
    width: parent.width
    height: parent.height

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: ttkMainMenubar
            anchors.right: parent.right
            anchors.top: parent.top
            width: ttkMainWindow.width
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.topbar_background

            RowLayout {
                id: mainMenubarLayout
                spacing: 2
                anchors.fill: parent

                Rectangle {
                    Text {
                        anchors.fill: parent
                        text:"\ue662"
                        font.family: "iconfont"
                        font.pixelSize: dpWidth(24)
                    }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: {

                        }
                    }
                    Layout.preferredWidth: dpWidth(24)
                    Layout.preferredHeight: dpHeight(24)
                    anchors.left: parent.left
                    anchors.leftMargin: dpWidth(10)
                    color: ttkTheme.alphaLv8
                }

                Rectangle {
                    Text {
                        anchors.fill: parent
                        text:"\ue60b"
                        font.family: "iconfont"
                        font.pixelSize: dpWidth(24)
                    }
                    MouseArea {
                        anchors.fill: parent
                        onPressed: {

                        }
                    }
                    Layout.preferredWidth: dpWidth(24)
                    Layout.preferredHeight: dpHeight(24)
                    anchors.right: parent.right
                    anchors.rightMargin: dpWidth(10)
                    color: ttkTheme.alphaLv8
                }
            }
        }

        ///main body
        Rectangle {
            anchors.right: ttkMainWindow.right
            anchors.top: ttkMainWindow.top
            width: ttkMainWindow.width
            height: ttkMainStackView.height - ttkMainMenubar.height
            color: "#956422"

        }

    }
}
