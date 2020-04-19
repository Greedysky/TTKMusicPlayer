#ifndef QSYNCUTILS_H
#define QSYNCUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include "musicextrasglobaldefine.h"

/*! @brief The class of the sync cloud data item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QSyncUtils
{
public:
    /*!
     * Get authorization code.
     */
    static QString getAuthorizationCode(const QString &key, const QString &method,
                                        const TTKStringMap &headers, const QString &resource);
    /*!
     * Create sign for normal auth.
     */
    static QString createSignForNormalAuth(const QString &method, const QString &access, const QString &secret,
                                           const TTKStringMap &headers, const QString &resource);
    /*!
     * Get GMT.
     */
    static QString getGMT();
    
    /*!
     * Generate data bucket url.
     */
    static QString generateDataBucketUrl();

};

#endif
