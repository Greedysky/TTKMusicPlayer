#ifndef MUSICCONTEXTUIOBJECT_H
#define MUSICCONTEXTUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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
        static constexpr const char *ContextPlay = " \
            QToolButton{ border:none; \
            background-image:url(:/contextMenu/btn_play_hover); } \
            QToolButton:hover{ background-image:url(:/contextMenu/btn_play_normal); }";

        static constexpr const char *ContextPause = " \
            QToolButton{ border:none; \
            background-image:url(:/contextMenu/btn_pause_hover); } \
            QToolButton:hover{ background-image:url(:/contextMenu/btn_pause_normal); }";

        static constexpr const char *ContextNext = " \
            QToolButton{ border:none; \
            background-image:url(:/contextMenu/btn_next_hover); } \
            QToolButton:hover{ background-image:url(:/contextMenu/btn_next_normal); }";

        static constexpr const char *ContextPrevious = " \
            QToolButton{ border:none; \
            background-image:url(:/contextMenu/btn_previous_hover); } \
            QToolButton:hover{ background-image:url(:/contextMenu/btn_previous_normal); }";

    }
}

#endif // MUSICCONTEXTUIOBJECT_H
