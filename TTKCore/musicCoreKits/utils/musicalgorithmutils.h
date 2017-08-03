#ifndef MUSICALGORITHMUTILS_H
#define MUSICALGORITHMUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicglobaldefine.h"

#define ALG_DOWNLOAD_KEY    "5024FRSDF40"
#define ALG_URL_KEY         "GREEDYSKY"
#define ALG_PLUS_KEY        ";DF;45622e"
#define ALG_LOW_KEY         "123asd"

/*! @brief The class of the utils algorithm object namespace.
 * @author Greedysky <greedysky@163.com>
 */
namespace MusicUtils
{
    namespace Algorithm
    {
        MUSIC_SUPER_EXPORT QByteArray md5(const QByteArray &data);
        /*!
         * Get md5 algorithm.
         */
        MUSIC_SUPER_EXPORT QByteArray sha1(const QByteArray &data);
        /*!
         * Get sha1 algorithm.
         */

        MUSIC_SUPER_EXPORT QString mdII(const QString &data, bool encode);
        /*!
         * Get mdII(greedysky) algorithm.
         */
        MUSIC_SUPER_EXPORT QString mdII(const QString &data, const QString &key, bool encode);
        /*!
         * Get mdII(greedysky) algorithm.
         */
    }
}

#endif // MUSICALGORITHMUTILS_H
