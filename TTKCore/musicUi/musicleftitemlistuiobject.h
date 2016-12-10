#ifndef MUSICLEFTITEMLISTUIOBJECT_H
#define MUSICLEFTITEMLISTUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the left item list style
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
///left item list style
//////////////////////////////////////////////////////
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

    const QString MKGItemMore = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_more_normal);} \
            QToolButton:hover{ background-image: url(:/appTools/item_more_hover);}";

    const QString MKGItemMoreClicked = " \
            QToolButton{ border:none; \
            background-image: url(:/appTools/item_more_clicked);}";

}

#endif // MUSICLEFTITEMLISTUIOBJECT_H
