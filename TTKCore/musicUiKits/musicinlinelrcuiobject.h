#ifndef MUSICINLINELRCUIOBJECT_H
#define MUSICINLINELRCUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the inline lrc button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGInlineTranslation = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_translation_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_translation_hover);}";

    const QString MKGInlineMicrophone = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_microphone_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_microphone_hover);}";

    const QString MKGInlineMessage = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_message_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_message_hover);}";

    const QString MKGInlineMovie = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_mv_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_mv_hover);}";
            
    const QString MKGInlineStar = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/btn_star_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/btn_star_hover);} \
            QPushButton:pressed{ background-image: url(:/lrc/btn_star_clicked);}";
            
    const QString MKGInlineMakeUp = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/lb_make_up_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_make_up_hover);}";
            
    const QString MKGInlineMakeDown = " \
            QPushButton{ border:none; \
            background-image: url(:/lrc/lb_make_down_normal);} \
            QPushButton:hover{ background-image: url(:/lrc/lb_make_down_hover);}";

}

#endif // MUSICINLINELRCUIOBJECT_H
