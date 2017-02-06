#ifndef MUSICCONTEXTUIOBJECT_H
#define MUSICCONTEXTUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the context button style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGContextPlay = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_play_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_play_normal);}";

    const QString MKGContextPause = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_pause_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_pause_normal);}";

    const QString MKGContextNext = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_next_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_next_normal);}";

    const QString MKGContextPrevious = " \
            QToolButton{ border:none; \
            background-image: url(:/contextMenu/btn_previous_hover);} \
            QToolButton:hover{ background-image: url(:/contextMenu/btn_previous_normal);}";

}

#endif // MUSICCONTEXTUIOBJECT_H
