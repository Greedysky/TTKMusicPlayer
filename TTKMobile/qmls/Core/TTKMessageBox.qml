import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Rectangle {
    id: ttkMessageBox

    property string title
    property string content
    property alias okText: okButton.text
    property alias cancelText: cancelButton.text

    signal result(int val)

    function doModal() {
        ttkMessageBox.state = "visible";
        Qt.inputMethod.hide();
    }

    visible: false
    anchors.fill: parent
    color: ttkTheme.alphaLv12

    MouseArea {
        anchors.fill: parent
        onClicked: {
            ttkMessageBox.visible = false;
            ttkMessageBox.result(0);
        }
    }

    Rectangle {
        color: ttkTheme.topbar_background
        radius: 5
        width: 0.7*parent.width
        height: 0.3*parent.height
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
        Rectangle {
            anchors.fill: parent
            color: ttkTheme.alphaLv4
            radius: 5
        }

        Column {
            anchors.fill: parent
            Rectangle {
                radius: 5
                width: parent.width
                height: 0.25*parent.height
                color: ttkTheme.topbar_background
                Rectangle {
                    anchors.fill: parent
                    color: ttkTheme.alphaLv3
                    radius: 5
                }

                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: title
                }
            }

            Text {
                id: textText
                width: parent.width
                height: 0.5*parent.height
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                text: content
                color: "black"
            }
        }

        RowLayout {
            anchors.fill: parent
            Rectangle {
                width: parent.width/3
                height: 0.15*parent.height
                anchors {
                    left: parent.left
                    bottom: parent.bottom
                    leftMargin: dpWidth(width/7)
                    bottomMargin: dpWidth(height/6)
                }
                color: ttkTheme.topbar_background
                Text {
                    id: okButton
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMessageBox.visible = false;
                        ttkMessageBox.result(1);
                    }
                }
            }

            Rectangle {
                width: parent.width/3
                height: 0.15*parent.height
                anchors {
                    right: parent.right
                    bottom: parent.bottom
                    rightMargin: dpWidth(width/7)
                    bottomMargin: dpWidth(height/6)
                }
                color: ttkTheme.topbar_background
                Text {
                    id: cancelButton
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttkMessageBox.visible = false;
                        ttkMessageBox.result(0);
                    }
                }
            }
        }
    }


    states: State {
        name: "visible"
        PropertyChanges {
            target: ttkMessageBox;
            visible: true
        }
    }

    transitions: Transition {
        NumberAnimation {
            target: ttkMessageBox
            properties: "visible"
            duration: 250
        }
    }

    Component.onCompleted:
    {
        var docRoot = ttkMessageBox.parent;
        while(docRoot.parent)
        {
            docRoot = docRoot.parent;
        }
        ttkMessageBox.parent = docRoot;
    }
}
