#ifndef MUSICLEFTITEMLISTUIOBJECT_H
#define MUSICLEFTITEMLISTUIOBJECT_H

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

/*! @brief The namespace of the left item list style
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
//left item list style
    const QString MKGItemMusic = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_music_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_music_hover);}";

    const QString MKGItemMusicClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_music_clicked);}";

    const QString MKGItemFavourite = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_favorite_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_favorite_hover);}";

    const QString MKGItemFavouriteClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_favorite_clicked);}";

    const QString MKGItemRadio = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_radio_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_radio_hover);}";

    const QString MKGItemRadioClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_radio_clicked);}";

    const QString MKGItemMobile = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_mobile_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_mobile_hover);}";

    const QString MKGItemMobileClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_mobile_clicked);}";

    const QString MKGItemDownload = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_download_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_download_hover);}";

    const QString MKGItemDownloadClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_download_clicked);}";

}

#endif // MUSICLEFTITEMLISTUIOBJECT_H
