#ifndef QNUTILS_H
#define QNUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the qiniu utils.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNUtils
{
public:
    static QString urlSafeBase64Encode(const QByteArray &data);
    /*!
     * Url safe base64 encode.
     */
    static QByteArray urlSafeBase64Decode(const QString &data);
    /*!
     * Url safe base64 decode.
     */
    static QByteArray sha1(const QByteArray &data);
    /*!
     * Sha1 Encode.
     */
    static QByteArray hmacSha1(const QByteArray &data, const QByteArray &secretKey);
    /*!
     * Get hmac in sha1.
     */
    static QString escapeKey(const QString &key);
    /*!
     * Get escape key.
     */
    static bool isParamValid(const QString &param);
    /*!
     * Current parameter is valid or not.
     */
    static uint expireInSeconds(const uint seconds);
    /*!
     * Get expire in seconds.
     */
    static uint crc32(const QByteArray &data);
    /*!
     * Crc32 Encode.
     */
    static QString randomFileName();
    /*!
     * Get random file name.
     */
    static QString urlQuery(const QString &url);
    /*!
     * Get url query string.
     */

    static const char KEY_ESCAPE_RESERVE_BYTES[];
    static const char KEY_ESCAPE_UNRESERVE_BYTES[];

};

#endif // QNUTILS_H
