import QtQuick 2.5

Row {
    property ListView target;
    spacing: 20

    Repeater {
        opacity: 0.8
        model: target.model.count
        Rectangle {
            width: dpWidth(10)
            height: dpHeight(10)
            radius: dpWidth(6)
            color: target.currentIndex == index ? "white" : "grey"
            MouseArea {
                width: dpWidth(20)
                height: dpHeight(20)
                anchors.centerIn: parent
                onClicked: target.currentIndex = index
            }
        }
    }
}
