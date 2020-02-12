#ifndef MUSICFUNCTIONLISTUIOBJECT_H
#define MUSICFUNCTIONLISTUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MQSSFuncSongFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_fore_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/song_fore_hover); }";

    const QString MQSSFuncSongForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_fore_clicked); }";

    const QString MQSSFuncSongBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_back_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/song_back_hover); }";

    const QString MQSSFuncRadioFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_fore_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/radio_fore_hover); }";

    const QString MQSSFuncRadioForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_fore_clicked); }";

    const QString MQSSFuncRadioBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_back_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/radio_back_hover); }";

    const QString MQSSFuncListFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_fore_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/list_fore_hover); }";

    const QString MQSSFuncListForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_fore_clicked); }";

    const QString MQSSFuncListBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_back_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/list_back_hover); }";

    const QString MQSSFuncMVFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_fore_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/mv_fore_hover); }";

    const QString MQSSFuncMVForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_fore_clicked); }";

    const QString MQSSFuncMVBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_back_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/mv_back_hover); }";

    const QString MQSSFuncLiveFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_fore_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/live_fore_hover); }";

    const QString MQSSFuncLiveForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_fore_clicked); }";

    const QString MQSSFuncLiveBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_back_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/live_back_hover); }";

    const QString MQSSFuncLrcFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/lrc_fore_normal); } \
            QToolButton:hover{ background-image: url(:/funcTools/lrc_fore_hover); }";

    const QString MQSSFuncLrcForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/lrc_back_clicked); }";

}

#endif // MUSICFUNCTIONLISTUIOBJECT_H
