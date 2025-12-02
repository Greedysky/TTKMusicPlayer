#ifndef MUSICFUNCTIONLISTUIOBJECT_H
#define MUSICFUNCTIONLISTUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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
 ***************************************************************************/

#include <QObject>

/*! @brief The namespace of the application ui object.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    namespace UI
    {
        static constexpr const char *FuncSongFore = " \
            QToolButton{ border:none; \
            background-image:url(:/title/song_fore_normal); } \
            QToolButton:hover{ background-image:url(:/title/song_fore_hover); }";

        static constexpr const char *FuncSongForeClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/title/song_fore_clicked); }";

        static constexpr const char *FuncSongBack = " \
            QToolButton{ border:none; \
            background-image:url(:/title/song_back_normal); } \
            QToolButton:hover{ background-image:url(:/title/song_back_hover); }";

        static constexpr const char *FuncRadioFore = " \
            QToolButton{ border:none; \
            background-image:url(:/title/radio_fore_normal); } \
            QToolButton:hover{ background-image:url(:/title/radio_fore_hover); }";

        static constexpr const char *FuncRadioForeClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/title/radio_fore_clicked); }";

        static constexpr const char *FuncRadioBack = " \
            QToolButton{ border:none; \
            background-image:url(:/title/radio_back_normal); } \
            QToolButton:hover{ background-image:url(:/title/radio_back_hover); }";

        static constexpr const char *FuncListFore = " \
            QToolButton{ border:none; \
            background-image:url(:/title/list_fore_normal); } \
            QToolButton:hover{ background-image:url(:/title/list_fore_hover); }";

        static constexpr const char *FuncListForeClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/title/list_fore_clicked); }";

        static constexpr const char *FuncListBack = " \
            QToolButton{ border:none; \
            background-image:url(:/title/list_back_normal); } \
            QToolButton:hover{ background-image:url(:/title/list_back_hover); }";

        static constexpr const char *FuncMVFore = " \
            QToolButton{ border:none; \
            background-image:url(:/title/mv_fore_normal); } \
            QToolButton:hover{ background-image:url(:/title/mv_fore_hover); }";

        static constexpr const char *FuncMVForeClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/title/mv_fore_clicked); }";

        static constexpr const char *FuncMVBack = " \
            QToolButton{ border:none; \
            background-image:url(:/title/mv_back_normal); } \
            QToolButton:hover{ background-image:url(:/title/mv_back_hover); }";

        static constexpr const char *FuncLiveFore = " \
            QToolButton{ border:none; \
            background-image:url(:/title/live_fore_normal); } \
            QToolButton:hover{ background-image:url(:/title/live_fore_hover); }";

        static constexpr const char *FuncLiveForeClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/title/live_fore_clicked); }";

        static constexpr const char *FuncLiveBack = " \
            QToolButton{ border:none; \
            background-image:url(:/title/live_back_normal); } \
            QToolButton:hover{ background-image:url(:/title/live_back_hover); }";

        static constexpr const char *FuncLrcFore = " \
            QToolButton{ border:none; \
            background-image:url(:/title/lrc_fore_normal); } \
            QToolButton:hover{ background-image:url(:/title/lrc_fore_hover); }";

        static constexpr const char *FuncLrcForeClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/title/lrc_back_clicked); }";

    }
}

#endif // MUSICFUNCTIONLISTUIOBJECT_H
