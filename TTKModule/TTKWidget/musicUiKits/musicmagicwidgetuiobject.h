#ifndef MUSICMAGICWIDGETUIOBJECT_H
#define MUSICMAGICWIDGETUIOBJECT_H

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
        static const QString EnhanceOn = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_on_normal); } \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_on_hover); }";

        static const QString EnhanceOff = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_off_normal); } \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_off_hover); }";

        static const QString EnhanceClose = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_close_normal); } \
            QToolButton:hover{ background-image: url(:/enhance/btn_close_hover); }";

        static const QString EnhanceHelp = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_help_normal); } \
            QToolButton:hover{ background-image: url(:/enhance/btn_help_hover); }";

        static const QString EnhanceShare = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_share_normal); } \
            QToolButton:hover{ background-image: url(:/enhance/btn_share_hover); }";

        static const QString EnhanceTitle = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/lb_title_normal); } \
            QToolButton:hover{ background-image: url(:/enhance/lb_title_hover); }";

        static const QString EqualizerOn = " \
            QPushButton{ border:none; \
            background-image: url(:/enhance/btn_equalizer_on_normal); } \
            QPushButton:hover{ background-image: url(:/enhance/btn_equalizer_on_hover); }";

        static const QString EqualizerOff = " \
            QPushButton{ border:none; \
            background-image: url(:/enhance/btn_equalizer_off_normal); } \
            QPushButton:hover{ background-image: url(:/enhance/btn_equalizer_off_hover); }";

    }
}

#endif // MUSICMAGICWIDGETUIOBJECT_H
