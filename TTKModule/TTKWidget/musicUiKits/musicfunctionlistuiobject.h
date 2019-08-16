#ifndef MUSICFUNCTIONLISTUIOBJECT_H
#define MUSICFUNCTIONLISTUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

/*! @brief The namespace of the function list style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGFuncSongFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/song_fore_hover);}";

    const QString MKGFuncSongForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_fore_clicked);}";

    const QString MKGFuncSongBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/song_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/song_back_hover);}";

    const QString MKGFuncRadioFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/radio_fore_hover);}";

    const QString MKGFuncRadioForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_fore_clicked);}";

    const QString MKGFuncRadioBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/radio_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/radio_back_hover);}";

    const QString MKGFuncListFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/list_fore_hover);}";

    const QString MKGFuncListForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_fore_clicked);}";

    const QString MKGFuncListBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/list_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/list_back_hover);}";

    const QString MKGFuncMVFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/mv_fore_hover);}";

    const QString MKGFuncMVForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_fore_clicked);}";

    const QString MKGFuncMVBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/mv_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/mv_back_hover);}";

    const QString MKGFuncLiveFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/live_fore_hover);}";

    const QString MKGFuncLiveForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_fore_clicked);}";

    const QString MKGFuncLiveBack = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/live_back_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/live_back_hover);}";

    const QString MKGFuncLrcFore = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/lrc_fore_normal);} \
            QToolButton:hover{ background-image: url(:/funcTools/lrc_fore_hover);}";

    const QString MKGFuncLrcForeClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/funcTools/lrc_back_clicked);}";

}

#endif // MUSICFUNCTIONLISTUIOBJECT_H
