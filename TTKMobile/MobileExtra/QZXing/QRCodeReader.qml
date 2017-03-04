/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtMultimedia 5.5
import QZXing 2.3
import "Core"

Rectangle {
    id: qrCodeReader
    width: parent.width
    height: parent.height
    color: ttkTheme.color_black
    z: (timer.running) ? (2) : (0)

    property alias prompt: labelForPrompt.text

    signal tagFind(string tag)

    function startScan() {
        timer.start();
    }

    onVisibleChanged: {
        if(visible) {
            camera.start();
        }else {
            timer.stop();
            camera.stop();
        }
    }

    Timer {
        id: timer
        interval: 300
        repeat: false
        onTriggered: {
            console.log( decoder.decodeImageQML(output) );
        }
    }

    Camera {
        id: camera
        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceAuto
        flash.mode: Camera.FlashRedEyeReduction

        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }

        focus {
            focusMode: Camera.FocusContinuous
            focusPointMode: Camera.FocusPointCustom
            customFocusPoint: Qt.point(0.2, 0.2) // Focus relative to top-left corner
        }
    }

    VideoOutput {
        id: output
        anchors.fill: parent;
        source: camera
        rotation: 90
        focus: visible // to receive focus and capture key events when visible
    }

    QZXing {
        id: decoder
        enabledDecoders: QZXing.DecoderFormat_QR_CODE

        onDecodingFinished: {
            if (succeeded) {
                console.log("QRCode scan done!");
            } else {
                timer.start();
            }
        }

        onTagFound: {
            console.log("sssssssssssssssssssssssssss" + tag);
        }
    }

    Rectangle {
        width: parent.width
        height: (parent.height / 2) - ttkGlobal.dpHeight(100)
        color: "#55000000"
    }

    Rectangle {
        y: (parent.height / 2) + ttkGlobal.dpHeight(100)
        width: parent.width
        height: (parent.height / 2) - ttkGlobal.dpHeight(100)
        color: "#55000000"
    }

    Rectangle {
        y: (parent.height / 2) - ttkGlobal.dpHeight(100)
        width: parent.width / 2 - ttkGlobal.dpWidth(100)
        height: 200
        color: "#55000000"
    }

    Rectangle {
        x: (parent.width / 2) + ttkGlobal.dpWidth(100)
        y: (parent.height / 2) - ttkGlobal.dpHeight(100)
        width: parent.width / 2 - ttkGlobal.dpWidth(100)
        height: 200
        color: "#55000000"
    }

    Rectangle {
        id: centralRectangle
        width: parent.width/2
        height: width
        color: "#00000000"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        border.color: "#ffffff"

        Image {
            width: ttkGlobal.dpWidth(20)
            height: ttkGlobal.dpHeight(20)
            anchors.rightMargin: -ttkGlobal.dpWidth(5)
            anchors.bottom: parent.top
            anchors.bottomMargin: -ttkGlobal.dpHeight(15)
            anchors.right: parent.right
            source: "qrc:/QZXing/frame"
        }

        Image {
            width: ttkGlobal.dpWidth(20)
            height: ttkGlobal.dpHeight(20)
            rotation: 90
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -ttkGlobal.dpWidth(5)
            anchors.right: parent.right
            anchors.rightMargin: -ttkGlobal.dpHeight(5)
            source: "qrc:/QZXing/frame"
        }

        Image {
            width: ttkGlobal.dpWidth(20)
            height: ttkGlobal.dpHeight(20)
            rotation: 180
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -ttkGlobal.dpHeight(5)
            anchors.right: parent.left
            anchors.rightMargin: -ttkGlobal.dpWidth(15)
            source: "qrc:/QZXing/frame"
        }

        Image {
            width: ttkGlobal.dpWidth(20)
            height: ttkGlobal.dpHeight(20)
            rotation: 270
            anchors.bottom: parent.top
            anchors.bottomMargin: -ttkGlobal.dpHeight(15)
            anchors.right: parent.left
            anchors.rightMargin: -ttkGlobal.dpWidth(15)
            source: "qrc:/QZXing/frame"
        }

        Image {
            id: imageLightNeedle
            width: parent.width
            height: ttkGlobal.dpHeight(24)
            source: "qrc:/QZXing/lightNeedle"

            PropertyAnimation {
                target: imageLightNeedle;
                property: "y"
                from: 0
                to: parent.width/2 - imageLightNeedle.height
                duration: 4000
                loops: -1
                running: imageLightNeedle.visible
                easing.type: Easing.InOutCubic;
            }
        }
    }

    Text {
        id: labelForPrompt
        width: parent.width/2
        height: ttkGlobal.dpHeight(30)
        color: ttkTheme.color_white
        text: qsTr("将取景框对着二维码，即可自动扫描")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        anchors.top: centralRectangle.bottom
        anchors.topMargin: ttkGlobal.dpHeight(30)
        anchors.horizontalCenter: parent.horizontalCenter
        font.pixelSize: height/2
    }

    MouseArea {
        anchors.fill: parent
        onDoubleClicked: {
            ttkOutStackView.pop();
        }
    }
}
