#ifndef QNUTILS_H
#define QNUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include <QObject>
#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the qiniu utils.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNUtils
{
public:
    /*!
     * Url safe base64 encode.
     */
    static QString urlSafeBase64Encode(const QByteArray &data);
    /*!
     * Url safe base64 decode.
     */
    static QByteArray urlSafeBase64Decode(const QString &data);
    /*!
     * Sha1 Encode.
     */
    static QByteArray sha1(const QByteArray &data);
    /*!
     * Get hmac in sha1.
     */
    static QByteArray hmacSha1(const QByteArray &data, const QByteArray &secretKey);
    /*!
     * Get escape key.
     */
    static QString escapeKey(const QString &key);
    /*!
     * Current parameter is valid or not.
     */
    static bool isParamValid(const QString &param);
    /*!
     * Get expire in seconds.
     */
    static uint expireInSeconds(const uint seconds);
    /*!
     * Crc32 Encode.
     */
    static uint crc32(const QByteArray &data);
    /*!
     * Get random file name.
     */
    static QString randomFileName();
    /*!
     * Get url query string.
     */
    static QString urlQuery(const QString &url);

    static const char KEY_ESCAPE_RESERVE_BYTES[];
    static const char KEY_ESCAPE_UNRESERVE_BYTES[];

};

#endif // QNUTILS_H
