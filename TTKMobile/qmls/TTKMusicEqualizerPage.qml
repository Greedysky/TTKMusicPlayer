/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import "Core"

Rectangle {
    id: ttkMusicEqualizerPage
    height: parent.height
    width: parent.width
    color: "#262C3B"

    property variant eqParas: []
    property variant eqTypes: ["80", "120", "125", "250", "500", "1k", "2k", "4k", "8k", "16k"];
    Component.onCompleted: {
        eqParas.push(0, 0, 0, 0, 0, 0, 0, 0, 0, 0
                   , 0, 0, 0, 0, 0, 0,-4,-4,-4,-6
                   , 0, 0, 5, 3, 3, 3, 2, 0, 0, 0
                   , 6, 4, 1, 0, 0,-3,-4,-4, 0, 0
                   ,-6, 6, 6, 3, 2,-2,-5,-6,-8,-8
                   ,-6,-6,-6,-2, 1, 7,10,10,10,10
                   , 4, 3, 0,-4,-3, 1, 5, 7, 7, 7
                   , 3, 7, 3,-2,-1, 1, 3, 6, 8, 9
                   , 6, 6, 3, 3, 0,-3,-3,-3, 0, 0
                   ,-3, 0, 2, 3, 3, 3, 2, 1, 1, 1
                   ,-1, 3, 4, 5, 3, 0,-1,-1,-1,-1
                   , 0, 0, 0,-3, 0, 4, 3, 0, 0, 0
                   , 4, 4, 0, 0, 0, 0, 0, 0, 4, 4
                   , 5, 3,-3,-5,-2, 2, 5, 7, 7, 7
                   ,-2,-3,-2, 0, 2, 3, 5, 6, 7, 6
                   , 3, 1, 0,-1, 0, 2, 5, 6, 7, 7
                   , 2, 2, 1, 0,-2,-3,-2, 0, 2, 5
                   , 5, 3, 0,-3,-3, 0, 5, 6, 6, 5);

        if(TTK_PLAYER.getEqEffectEnable() === 1) {
            var index = TTK_PLAYER.getEqEffectIndex();
            currentEQTypeChanged(index);
        }
    }

    function currentEQTypeChanged(index) {
        if(index === 1 ) {
            return;
        }

        var values = new Array;
        values.push(0);
        eqTypeSliderListModel.clear()
        if(index === 0) {
            for(var i=0; i<eqTypes.length; ++i) {
                values.push(0);
                var info = {
                    title: eqTypes[i],
                    sliderValue: eqParas[i],
                    sliderEnable: false
                };
                eqTypeSliderListModel.append(info);
            }
        }else {
            for(var j=0; j<eqTypes.length; ++j) {
                values.push(eqParas[(index-2)*10 + j]);
                var info1 = {
                    title: eqTypes[j],
                    sliderValue: eqParas[(index-2)*10 + j],
                    sliderEnable: true
                };
                eqTypeSliderListModel.append(info1);
            }
        }
        TTK_PLAYER.setEqEffect(values);
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: mainMenubar
            Layout.fillWidth: true
            height: dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.alphaLv0

            RowLayout {
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: dpWidth(50)
                    Layout.preferredHeight: dpHeight(50)
                    anchors.left: parent.left
                    onPressed: {
                        ttkOutStackView.pop();
                    }
                }
            }
        }

        ///main body
        Rectangle {
            Layout.fillWidth: true
            height: ttkMusicEqualizerPage.height - mainMenubar.height
            color: ttkTheme.alphaLv0

            RowLayout {
                spacing: 0
                anchors.fill: parent

                ListView {
                    id: eqTypeListView
                    anchors {
                        left: parent.left
                        leftMargin: dpHeight(10)
                    }
                    Layout.fillHeight: true
                    Layout.preferredWidth: dpWidth(100)
                    orientation: ListView.Vertical
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    spacing: dpHeight(5)

                    delegate: Rectangle {
                        id: wrapper1
                        width: dpWidth(100)
                        height: dpHeight(60)
                        color: ttkTheme.alphaLv0

                        Text {
                            Layout.alignment: Qt.AlignCenter
                            verticalAlignment: Qt.AlignVCenter
                            width: parent.width
                            text: title
                            wrapMode: Text.WordWrap
                            color: parent.ListView.isCurrentItem ? "#00ff00" : ttkTheme.alphaLv6
                            font.bold: parent.ListView.isCurrentItem
                        }

                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                eqTypeListView.currentIndex = index;
                                currentEQTypeChanged(index);
                            }
                        }
                    }

                    model: ListModel {
                        ListElement {
                            title: qsTr("关闭")
                        }
                        ListElement {
                            title: qsTr("自定义")
                        }
                        ListElement {
                            title: qsTr("默认")
                        }
                        ListElement {
                            title: qsTr("古典")
                        }
                        ListElement {
                            title: qsTr("俱乐部")
                        }
                        ListElement {
                            title: qsTr("舞曲")
                        }
                        ListElement {
                            title: qsTr("增强低音")
                        }
                        ListElement {
                            title: qsTr("增强高音")
                        }
                        ListElement {
                            title: qsTr("增强高低音")
                        }
                        ListElement {
                            title: qsTr("耳机")
                        }
                        ListElement {
                            title: qsTr("大厅")
                        }
                        ListElement {
                            title: qsTr("现场")
                        }
                        ListElement {
                            title: qsTr("流行")
                        }
                        ListElement {
                            title: qsTr("瑞格")
                        }
                        ListElement {
                            title: qsTr("派对")
                        }
                        ListElement {
                            title: qsTr("摇滚")
                        }
                        ListElement {
                            title: qsTr("斯卡")
                        }
                        ListElement {
                            title: qsTr("温和")
                        }
                        ListElement {
                            title: qsTr("温和摇滚")
                        }
                        ListElement {
                            title: qsTr("电子")
                        }
                    }

                    Component.onCompleted: {
                        eqTypeListView.currentIndex = 0;
                    }
                }

                ListView {
                    id: eqTypeSliderListView
                    anchors {
                        left: eqTypeListView.right
                        leftMargin: dpWidth(10)
                    }
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: true
                    Layout.preferredWidth: ttkMusicEqualizerPage.width - eqTypeListView.width - dpWidth(10)
                    orientation: ListView.Vertical
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    spacing: dpHeight(5)

                    delegate: Rectangle {
                        id: wrapper
                        width: dpWidth(ttkMusicEqualizerPage.width - eqTypeListView.width) - dpWidth(10)
                        height: dpHeight(55)
                        color: ttkTheme.alphaLv0

                        Text {
                            id: sliderText
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignLeft
                            color: ttkTheme.alphaLv6
                            width: parent.width/6
                            text: title
                            wrapMode: Text.WordWrap
                        }

                        Slider {
                            id: eqValueSlider
                            anchors {
                                left: sliderText.right
                                leftMargin: dpWidth(5)
                            }
                            width: parent.width*0.9
                            height: sliderText.height
                            minimumValue: -16
                            maximumValue: 16
                            value: sliderValue
                            enabled: sliderEnable

                            function sliderGeometry() {
                                return (eqValueSlider.value - eqValueSlider.minimumValue) /
                                       (eqValueSlider.maximumValue - eqValueSlider.minimumValue);
                            }

                            style: SliderStyle{
                                groove: Row {
                                    Rectangle{
                                        implicitWidth: eqValueSlider.width*eqValueSlider.sliderGeometry()
                                        implicitHeight: dpHeight(3)
                                        color: ttkTheme.topbar_background
                                    }

                                    Rectangle{
                                        implicitWidth: eqValueSlider.width*(1-eqValueSlider.sliderGeometry())
                                        implicitHeight: dpHeight(3)
                                        color: "gray"
                                    }
                                }

                                handle: Rectangle{
                                    anchors.centerIn: parent;
                                    color: ttkTheme.topbar_background
                                    width: dpWidth(20)
                                    height: dpHeight(20)
                                    radius: dpWidth(10)
                                }
                            }
                        }
                    }

                    model: ListModel {
                        id: eqTypeSliderListModel
                    }
                }
            }
        }
    }
}
