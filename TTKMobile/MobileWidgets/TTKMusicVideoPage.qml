/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

import QtQuick 2.5
import QtMultimedia 5.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import "Core"

Rectangle{
    id: ttkMusicVideoPage
    width: parent.width
    height: parent.height

    Column {
        Rectangle {
            color: "black"
            width: ttkMusicVideoPage.width
            height: ttkMusicVideoPage.height - ttkGlobal.dpHeight(50)

            MediaPlayer {
                id: videoPlayer
                source: ttkGlobal.video_url
                autoPlay: true
                volume: 1

                onPositionChanged: {
                    videoTimeSlider.value = position;
                }
            }

            VideoOutput {
                anchors.fill: parent
                source: videoPlayer
            }
        }

        Rectangle {
            color: ttkTheme.color_alpha_lv15
            width: ttkMusicVideoPage.width
            height: ttkGlobal.dpHeight(50)

            Row {
                spacing: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: ttkGlobal.dpWidth(10)
                anchors.left: parent.left

                TTKImageButton {
                    width: height*1.2
                    height: ttkGlobal.dpHeight(40)
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/landscape_player_btn_pause_normal"

                    property int status: 1
                    onClicked: {
                        if(status === 1) {
                            videoPlayer.pause();
                            status = 0;
                            source = "qrc:/image/landscape_player_btn_play_normal";
                        }else {
                            videoPlayer.play();
                            status = 1;
                            source = "qrc:/image/landscape_player_btn_pause_normal";
                        }
                    }
                }

                Text {
                    width: ttkGlobal.dpWidth(60)
                    id: positionLabel
                    text: TTK_UTILS.normalizeTime(videoPlayer.position, "mm:ss");
                    color: ttkTheme.color_white
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                }

                Slider {
                    id: videoTimeSlider
                    width: ttkMusicVideoPage.width - ttkGlobal.dpWidth(30)*8
                    height: ttkGlobal.dpHeight(30)
                    minimumValue: 0
                    maximumValue: videoPlayer.duration <= 0 ? 1 : videoPlayer.duration
                    anchors.verticalCenter: parent.verticalCenter

                    function sliderGeometry() {
                        return (videoTimeSlider.value - videoTimeSlider.minimumValue) /
                               (videoTimeSlider.maximumValue - videoTimeSlider.minimumValue);
                    }

                    style: SliderStyle{
                        groove: Row {
                            Rectangle{
                                implicitWidth: videoTimeSlider.width*videoTimeSlider.sliderGeometry()
                                implicitHeight: ttkGlobal.dpHeight(3)
                                color: ttkTheme.topbar_background
                            }

                            Rectangle{
                                implicitWidth: videoTimeSlider.width*(1-videoTimeSlider.sliderGeometry())
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
                    }

                    MouseArea {
                        id: videoTimeSliderArea
                        anchors.fill: parent
                        onClicked: {
                            if(videoPlayer.seekable) {
                                var pos = videoPlayer.duration * mouse.x / parent.width;
                            }
                            videoPlayer.seek(pos);
                        }
                    }
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignHCenter
                    width: ttkGlobal.dpWidth(60)
                    id: durationLabel
                    text: TTK_UTILS.normalizeTime(videoPlayer.duration, "mm:ss");
                    color: ttkTheme.color_white
                }
            }
        }
    }
}
