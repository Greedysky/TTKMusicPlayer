#ifndef MUSICMAGICWIDGETUIOBJECT_H
#define MUSICMAGICWIDGETUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the music magic widget style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGEnhanceOn = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_on_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_on_hover);}";

    const QString MKGEnhanceOff = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_magic_off_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_magic_off_hover);}";

    const QString MKGEnhanceClose = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_close_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_close_hover);}";

    const QString MKGEnhanceHelp = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_help_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_help_hover);}";

    const QString MKGEnhanceShare = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/btn_share_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/btn_share_hover);}";

    const QString MKGEnhanceTitle = " \
            QToolButton{ border:none; \
            background-image: url(:/enhance/lb_title_normal);} \
            QToolButton:hover{ background-image: url(:/enhance/lb_title_hover);}";

    const QString MKGEqualizerOn = " \
            QPushButton{ border:none; \
            background-image: url(:/enhance/btn_equalizer_on_normal);} \
            QPushButton:hover{ background-image: url(:/enhance/btn_equalizer_on_hover);}";

    const QString MKGEqualizerOff = " \
            QPushButton{ border:none; \
            background-image: url(:/enhance/btn_equalizer_off_normal);} \
            QPushButton:hover{ background-image: url(:/enhance/btn_equalizer_off_hover);}";

}

#endif // MUSICMAGICWIDGETUIOBJECT_H
