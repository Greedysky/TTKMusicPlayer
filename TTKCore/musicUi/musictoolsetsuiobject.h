#ifndef MUSICTOOLSETSUIOBJECT_H
#define MUSICTOOLSETSUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the music tool sets button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGSongsDetectStartBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_hover);}";

    const QString MKGSongsDetectStopBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_detect_stop_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_detect_stop_hover);}";

    const QString MKGSongsRedetectBtn = " \
            QPushButton{ border:none; \
            background-image: url(:/toolSets/btn_redetect_normal);} \
            QPushButton:hover{ background-image: url(:/toolSets/btn_redetect_hover);} \
            QPushButton:disabled{ background-image: url(:/toolSets/btn_redetect_hover);}";

}

#endif // MUSICTOOLSETSUIOBJECT_H
