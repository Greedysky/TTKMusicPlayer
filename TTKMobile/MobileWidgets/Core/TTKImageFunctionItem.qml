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
    height: ttkGlobal.dpHeight(60)
    width: parent.width
    color: ttkTheme.color_alpha_lv0

    property alias textColor: text.color
    property alias text: text.text
    property alias source: image.foreground
    property alias subSource: subImage.source

    signal imageButtonPressed

    RowLayout {
        spacing: 2
        anchors.fill: parent

        TTKRadiusImage {
            id: image
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: ttkGlobal.dpWidth(50)
            Layout.preferredHeight: ttkGlobal.dpHeight(50)
            anchors {
                left: parent.left
                leftMargin: ttkGlobal.dpHeight(10)
            }
            color: ttkTheme.color_alpha_lv0
            foreground: "qrc:/image/default_background"
            background: "qrc:/image/radius_mask"
        }

        Text {
            id: text
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: ttkImageFunctionItem.width - ttkGlobal.dpHeight(120)
            verticalAlignment: Qt.AlignHCenter
            font.pixelSize: ttkImageFunctionItem.height/3
            anchors {
                left: image.right
                leftMargin: ttkGlobal.dpHeight(10)
            }
            elide: Text.ElideRight
            color: ttkTheme.color_white
        }

        TTKImageButton {
            id: subImage
            source: "qrc:/image/ic_toolbar_advance"
            width: ttkGlobal.dpWidth(50)
            height: ttkGlobal.dpHeight(50)
            anchors.right: parent.right
            onPressed: {
                ttkImageFunctionItem.imageButtonPressed();
            }
        }
    }

    Rectangle {
        width: ttkImageFunctionItem.width - image.width - ttkGlobal.dpHeight(5)
        height: 1
        anchors {
            left: parent.left
            top: parent.bottom
            leftMargin: ttkGlobal.dpWidth(50)
        }
        color: ttkTheme.color_gray
    }
}
