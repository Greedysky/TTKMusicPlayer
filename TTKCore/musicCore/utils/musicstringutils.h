#ifndef MUSICSTRINGUTILS_H
#define MUSICSTRINGUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicnumberdefine.h"
#include "musicglobaldefine.h"

/*! @brief The class of the utils object namespace.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace String
    {
        MUSIC_CORE_EXPORT QList<QColor> readColorConfig(const QString &value);
        /*!
         * Transform colors string to color list.
         */
        MUSIC_CORE_EXPORT QString writeColorConfig(const QColor &color);
        /*!
         * Transform color to color string.
         */
        MUSIC_CORE_EXPORT QString writeColorConfig(const QList<QColor> &colors);
        /*!
         * Transform color list to colors string.
         */
    }
}

#endif // MUSICSTRINGUTILS_H
