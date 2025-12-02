#ifndef MUSICLEFTITEMLISTUIOBJECT_H
#define MUSICLEFTITEMLISTUIOBJECT_H

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
        static constexpr const char *ItemMusic = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_music_normal); } \
            QToolButton:hover{ background-image:url(:/navigation/item_music_hover); }";

        static constexpr const char *ItemMusicClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_music_clicked); }";

        static constexpr const char *ItemLocal = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_local_normal); } \
            QToolButton:hover{ background-image:url(:/navigation/item_local_hover); }";

        static constexpr const char *ItemLocalClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_local_clicked); }";

        static constexpr const char *ItemCloud = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_cloud_normal); } \
            QToolButton:hover{ background-image:url(:/navigation/item_cloud_hover); }";

        static constexpr const char *ItemCloudClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_cloud_clicked); }";

        static constexpr const char *ItemRadio = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_radio_normal); } \
            QToolButton:hover{ background-image:url(:/navigation/item_radio_hover); }";

        static constexpr const char *ItemRadioClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_radio_clicked); }";

        static constexpr const char *ItemDownload = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_download_normal); } \
            QToolButton:hover{ background-image:url(:/navigation/item_download_hover); }";

        static constexpr const char *ItemDownloadClicked = " \
            QToolButton{ border:none; \
            background-image:url(:/navigation/item_download_clicked); }";

    }
}

#endif // MUSICLEFTITEMLISTUIOBJECT_H
