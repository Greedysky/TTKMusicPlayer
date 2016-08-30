#ifndef MUSICTINYUIOBJECT_H
#define MUSICTINYUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the tiny button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGTinyBtnPlay = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_play_hover);}";

    const QString MKGTinyBtnPause = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_pause_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_pause_hover);}";

    const QString MKGTinyBtnNext = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_next_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_next_hover);}";

    const QString MKGTinyBtnPrevious = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_previous_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_previous_hover);}";

    const QString MKGTinyBtnSetting = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_setting_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_setting_hover);}";

    const QString MKGTinyBtnUnDownload = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_download_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_download_hover);}";

    const QString MKGTinyBtnDownload = MKGTinyBtnUnDownload + " \
            QPushButton{ margin-left:-16px; }";

    const QString MKGTinyBtnUnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_unloved_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_unloved_hover);}";

    const QString MKGTinyBtnLove = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_loved_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_loved_hover);}";

    const QString MKGTinyBtnDelete = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_delete_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_delete_hover);}";

    const QString MKGTinyBtnMV = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_mv_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_mv_hover);}";

    const QString MKGTinyBtnPlayLater = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_play_later_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_play_later_hover);}";

    const QString MKGTinyBtnMore = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_more_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_more_hover);}";

    const QString MKGTinyBtnLocalSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_hover);}";

    const QString MKGTinyBtnMainSearch = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_search_main_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_search_main_hover);}";

    const QString MKGTinyBtnLocation = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_location_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_location_hover);}";

    const QString MKGTinyBtnSound = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound_hover);}";

    const QString MKGTinyBtnSoundWhite = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_sound);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_sound);}";

    const QString MKGTinyBtnClose = " \
            QToolButton{ border:none; \
            background-image: url(:/tiny/btn_close_normal);} \
            QToolButton:hover{ background-image: url(:/tiny/btn_close_hover);}";

    const QString MKGTinyBtnLrcExpand = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_collapse_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_collapse_hover);}";

    const QString MKGTinyBtnLrcCollapse = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_expand_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_expand_hover);}";

    const QString MKGTinyBtnWintopOn = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_top_on_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_top_on_hover);}";

    const QString MKGTinyBtnWintopOff = " \
            QPushButton{ border:none; \
            background-image: url(:/tiny/btn_top_off_normal);} \
            QPushButton:hover{ background-image: url(:/tiny/btn_top_off_hover);}";

}

#endif // MUSICTINYUIOBJECT_H
