/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

import QtQuick 2.5
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import "Core"

Rectangle {
    id: ttkMusicEqualizerPage
    height: parent.height
    width: parent.width
    color: "#262C3B"

    property variant eqParas: [0, 0, 0, 0, 0, 0, 0, 0, 0, 0
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
                             , 5, 3, 0,-3,-3, 0, 5, 6, 6, 5]
    property variant presetEQ: [10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    property variant eqTypes: ["80", "120", "125", "250", "500", "1k", "2k", "4k", "8k", "16k"];

    function changedEQValue(index, value) {
        presetEQ[index + 1] = parseInt(value);
        TTK_PLAYER.setEqEffect(presetEQ);
    }

    function currentEQTypeChanged(index) {
        eqTypeSliderListModel.clear();
        if(index === 1 ) {
            for(var i=0; i<eqTypes.length; ++i) {
                var info0 = {
                    title: eqTypes[i],
                    sliderValue: presetEQ[i + 1].toString(),
                    sliderEnable: true
                };
                eqTypeSliderListModel.append(info0);
            }
        }else if(index === 0) {
            for(var j=0; j<eqTypes.length; ++j) {
                presetEQ[j + 1] = 0;
                var info1 = {
                    title: eqTypes[j],
                    sliderValue: presetEQ[j + 1].toString(),
                    sliderEnable: false
                };
                eqTypeSliderListModel.append(info1);
            }
        }else {
            for(var k=0; k<eqTypes.length; ++k) {
                presetEQ[k + 1] = eqParas[(index-2)*10 + k];
                var info2 = {
                    title: eqTypes[k],
                    sliderValue: presetEQ[k + 1].toString(),
                    sliderEnable: true
                };
                eqTypeSliderListModel.append(info2);
            }
        }
        TTK_PLAYER.setEqEffect(presetEQ);
    }

    ColumnLayout {
        spacing: 0
        anchors.fill: parent

        ///top bar
        Rectangle {
            id: mainMenubar
            Layout.fillWidth: true
            height: ttkGlobal.dpHeight(ttkTheme.topbar_height)
            color: ttkTheme.color_alpha_lv0

            RowLayout {
                anchors.fill: parent

                TTKImageButton {
                    source: "qrc:/image/title_bar_back"
                    Layout.preferredWidth: ttkGlobal.dpWidth(50)
                    Layout.preferredHeight: ttkGlobal.dpHeight(50)
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
            color: ttkTheme.color_alpha_lv0

            RowLayout {
                spacing: 0
                anchors.fill: parent

                ListView {
                    id: eqTypeListView
                    anchors {
                        left: parent.left
                        leftMargin: ttkGlobal.dpHeight(10)
                    }
                    Layout.fillHeight: true
                    Layout.preferredWidth: ttkGlobal.dpWidth(100)
                    orientation: ListView.Vertical
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    spacing: ttkGlobal.dpHeight(5)

                    delegate: Rectangle {
                        id: wrapper1
                        width: ttkGlobal.dpWidth(100)
                        height: ttkGlobal.dpHeight(60)
                        color: ttkTheme.color_alpha_lv0

                        Text {
                            Layout.alignment: Qt.AlignCenter
                            verticalAlignment: Qt.AlignVCenter
                            width: parent.width
                            text: title
                            wrapMode: Text.WordWrap
                            color: parent.ListView.isCurrentItem ? ttkTheme.color_green : ttkTheme.color_alpha_lv6
                            font.bold: parent.ListView.isCurrentItem
                        }

                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                TTK_PLAYER.setEqEffectIndex(index);
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
                        if(TTK_PLAYER.getEqEffectEnable() === 1) {
                            var index = TTK_PLAYER.getEqEffectIndex();
                            eqTypeListView.currentIndex = index;
                            if(index === 1) {
                                var strValues = TTK_PLAYER.getEqEffectValue();
                                for(var i=0; i<strValues.length; ++i) {
                                    presetEQ[i + 1] = strValues[i];
                                }
                            }
                            currentEQTypeChanged(index);
                        }
                    }
                }

                ListView {
                    id: eqTypeSliderListView
                    anchors {
                        left: eqTypeListView.right
                        leftMargin: ttkGlobal.dpWidth(10)
                    }
                    Layout.alignment: Qt.AlignCenter
                    Layout.fillHeight: true
                    Layout.preferredWidth: ttkGlobal.dpWidth(ttkMusicEqualizerPage.width) - ttkGlobal.dpWidth(10 + 100)
                    orientation: ListView.Vertical
                    boundsBehavior: Flickable.StopAtBounds
                    clip: true
                    spacing: ttkGlobal.dpHeight(5)

                    delegate: Rectangle {
                        id: wrapper
                        width: ttkGlobal.dpWidth(50*8)
                        height: ttkGlobal.dpHeight(55)
                        color: ttkTheme.color_alpha_lv0

                        Text {
                            id: sliderText
                            verticalAlignment: Qt.AlignVCenter
                            horizontalAlignment: Qt.AlignLeft
                            color: ttkTheme.color_alpha_lv6
                            width: ttkGlobal.dpWidth(50)
                            text: title
                            wrapMode: Text.WordWrap
                        }

                        Slider {
                            id: eqValueSlider
                            anchors {
                                left: sliderText.right
                                leftMargin: ttkGlobal.dpWidth(5)
                            }
                            width: wrapper.width*0.5
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
                                        implicitHeight: ttkGlobal.dpHeight(3)
                                        color: ttkTheme.topbar_background
                                    }

                                    Rectangle{
                                        implicitWidth: eqValueSlider.width*(1-eqValueSlider.sliderGeometry())
                                        implicitHeight: ttkGlobal.dpHeight(3)
                                        color: ttkTheme.color_gray
                                    }
                                }

                                handle: Rectangle{
                                    anchors.centerIn: parent;
                                    color: ttkTheme.topbar_background
                                    width: ttkGlobal.dpWidth(20)
                                    height: ttkGlobal.dpHeight(20)
                                    radius: ttkGlobal.dpWidth(10)
                                }

                                panel: Rectangle{
                                    anchors.fill: parent;
                                    color: ttkTheme.color_alpha_lv0

                                    Loader{
                                        id: grooveLoader
                                        anchors.centerIn: parent
                                        sourceComponent: groove
                                    }

                                    Loader{
                                        id: handleLoader
                                        anchors.verticalCenter: grooveLoader.verticalCenter;
                                        x: Math.min(grooveLoader.x + ((control.value + 16)*grooveLoader.width)/(control.maximumValue - control.minimumValue) - item.width/2,
                                                    grooveLoader.width)
                                        sourceComponent: handle
                                        onXChanged: {
                                            var value = 32/parent.width*x - 14.5;
                                            changedEQValue(index, value);
                                        }
                                    }
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
