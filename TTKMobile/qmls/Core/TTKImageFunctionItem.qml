/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Layouts 1.1

Rectangle {
    id: ttkImageFunctionItem
    height: dpHeight(60)
    width: parent.width
    color: ttkTheme.alphaLv0

    property alias text: text.text
    property alias source: image.foreground

    RowLayout {
        spacing: 2
        anchors.fill: parent

        TTKRadiusImage {
            id: image
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: dpWidth(50)
            Layout.preferredHeight: dpHeight(50)
            anchors {
                left: parent.left
                leftMargin: dpHeight(10)
            }
            color: ttkTheme.alphaLv0
            foreground: "qrc:/image/test"
            background: "qrc:/image/radius_mask"
        }

        Text {
            id: text
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: ttkImageFunctionItem.width - dpHeight(120)
            verticalAlignment: Qt.AlignHCenter
            font.pixelSize: ttkImageFunctionItem.height/3
            anchors {
                left: image.right
                leftMargin: dpHeight(10)
            }
            color: ttkTheme.white
            elide: Text.ElideRight
        }

        TTKImageButton {
            source: "qrc:/image/ic_toolbar_advance"
            Layout.preferredWidth: dpWidth(50)
            Layout.preferredHeight: dpHeight(50)
            anchors.right: parent.right
            onPressed: {

            }
        }
    }

    Rectangle {
        width: ttkImageFunctionItem.width - image.width
        height: 1
        anchors {
            left: parent.left
            top: parent.bottom
            leftMargin: dpHeight(20 + image.width)
        }
        color: "gray"
    }
}
