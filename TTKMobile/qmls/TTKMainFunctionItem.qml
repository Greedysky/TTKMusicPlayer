import QtQuick 2.5
import QtQuick.Layouts 1.1

Rectangle {
    id: ttkMainFunctionItem
    width: parent.width/3
    height: (parent.height-10)/2

    property alias source: image.source
    property alias mainTitle: mainTitle.text
    property alias subTitle: subTitle.text

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        Rectangle {
            Layout.preferredWidth: dpWidth(80)
            Layout.preferredHeight: dpHeight(80)
            Layout.alignment: Qt.AlignCenter
            color: ttkTheme.alphaLv0

            Image {
                id: image
                anchors.fill: parent
            }
        }

        Text {
            id: mainTitle
            Layout.alignment: Qt.AlignCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 15
        }

        Text {
            id: subTitle
            Layout.alignment: Qt.AlignCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "gray"
        }
    }
}
