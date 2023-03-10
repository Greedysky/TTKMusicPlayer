#ifndef MUSICLEFTITEMLISTUIOBJECT_H
#define MUSICLEFTITEMLISTUIOBJECT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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
        const QString ItemMusic = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_music_normal); } \
                QToolButton:hover{ background-image: url(:/navigation/item_music_hover); }";

        const QString ItemMusicClicked = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_music_clicked); }";

        const QString ItemLocal = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_local_normal); } \
                QToolButton:hover{ background-image: url(:/navigation/item_local_hover); }";

        const QString ItemLocalClicked = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_local_clicked); }";

        const QString ItemCloud = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_cloud_normal); } \
                QToolButton:hover{ background-image: url(:/navigation/item_cloud_hover); }";

        const QString ItemCloudClicked = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_cloud_clicked); }";

        const QString ItemRadio = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_radio_normal); } \
                QToolButton:hover{ background-image: url(:/navigation/item_radio_hover); }";

        const QString ItemRadioClicked = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_radio_clicked); }";

        const QString ItemDownload = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_download_normal); } \
                QToolButton:hover{ background-image: url(:/navigation/item_download_hover); }";

        const QString ItemDownloadClicked = " \
                QToolButton{ border:none; \
                background-image: url(:/navigation/item_download_clicked); }";

    }
}

#endif // MUSICLEFTITEMLISTUIOBJECT_H
