#ifndef MUSICNUMBERUTILS_H
#define MUSICNUMBERUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicnumberdefine.h"
#include "musicglobaldefine.h"

/*! @brief The class of the utils number object namespace.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace Number
    {
        MUSIC_CORE_EXPORT QString size2Number(qint64 size);
        /*!
         * Transform value to cp float value.
         */
        MUSIC_CORE_EXPORT QString size2NumberInt(qint64 size);
        /*!
         * Transform value to cp int value.
         */
        MUSIC_CORE_EXPORT QString size2Label(qint64 size);
        /*!
         * Transform value to cp value.
         */
        MUSIC_CORE_EXPORT QString speed2Label(qint64 size);
        /*!
         * Transform value to speed float value.
         */
        MUSIC_CORE_EXPORT QString speed2LabelInt(qint64 size);
        /*!
         * Transform value to speed int value.
         */

        MUSIC_CORE_EXPORT qreal sizeByte2KByte(qint64 size);
        /*!
         * Transform value to kb entity.
         */
        MUSIC_CORE_EXPORT qreal sizeByte2MByte(qint64 size);
        /*!
         * Transform value to mb entity.
         */
        MUSIC_CORE_EXPORT qreal sizeByte2TByte(qint64 size);
        /*!
         * Transform value to tb entity.
         */

        MUSIC_CORE_EXPORT QString speed2LabelFromLabel(qint64 size, const QString &label);
        /*!
         * Transform value to speed value base.
         */
    }
}

#endif // MUSICNUMBERUTILS_H
