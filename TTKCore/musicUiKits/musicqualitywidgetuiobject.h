#ifndef MUSICQUALITYWIDGETUIOBJECT_H
#define MUSICQUALITYWIDGETUIOBJECT_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>

/*! @brief The namespace of the music quality widget style.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUIObject
{
    const QString MKGBtnQuality = " \
            QToolButton{ border:none; \
            background-image: url(:/quality/btn_quality_normal);} \
            QToolButton:hover{ background-image: url(:/quality/btn_quality_hover);}";

}

#endif // MUSICQUALITYWIDGETUIOBJECT_H
