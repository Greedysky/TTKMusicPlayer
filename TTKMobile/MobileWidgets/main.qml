/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Window 2.0
import "Core"

ApplicationWindow {
    id: ttkMainWindow
    visible: true
    width: ttkGlobal.window_width
    height: ttkGlobal.window_height

    property double preQuitTime: 0

    TTKTheme {
        id: ttkTheme
    }

    TTKGlobal {
        id: ttkGlobal
    }

    StackView {
        id: ttkOutStackView
        width: parent.width
        height: parent.height
        focus: true
        initialItem: Column {
            spacing: 0

            StackView {
                id: ttkMainStackView
                width: ttkMainWindow.width
                height: ttkMainWindow.height - ttkMusicBar.height
                focus: true
                initialItem: TTKMainPage {
                }
            }

            TTKMusicBar {
                id: ttkMusicBar
            }
        }
    }

    onClosing: {
        if(ttkOutStackView.depth > 1) {
            ttkOutStackView.pop();
            close.accepted = false;
        }else {
            if(ttkMainStackView.depth > 1) {
                ttkMainStackView.pop();
                close.accepted = false;
            }else {
                if(ttkGlobal.isAndroid()) {
                    var nowQuitTime = Date.parse(new Date);
                    if(preQuitTime === 0) {
                        preQuitTime = nowQuitTime;
                        close.accepted = false;
                    }else {
                        if(nowQuitTime - preQuitTime <= 1200) {
                            close.accepted = true;
                        }else {
                            preQuitTime = nowQuitTime;
                            close.accepted = false;
                        }
                    }
                    TTK_UTILS.showMessageBox(qsTr("再按一次退出"));
                }else {
                    close.accepted = true;
                }
            }
        }
    }
}
