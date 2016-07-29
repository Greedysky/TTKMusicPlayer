#ifndef QNUTILS_H
#define QNUTILS_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicextrasglobaldefine.h"

class MUSIC_EXTRAS_EXPORT QNUtils
{
public:
    QNUtils();
    ~QNUtils();

    static QString urlSafeBase64Encode(const QByteArray &data);
    static QByteArray urlSafeBase64Decode(const QString &data);
    static QByteArray sha1(const QByteArray &data);
    static QByteArray hmacSha1(const QByteArray &data, const QByteArray &secretKey);
    static QString escapeKey(const QString &key);
    static bool isParamValid(const QString &param);
    static uint expireInSeconds(const uint seconds);
    static uint crc32(const QByteArray &data);
    static QString randomFileName();
    static QString urlQuery(const QString &url);

    static const char KEY_ESCAPE_RESERVE_BYTES[];
    static const char KEY_ESCAPE_UNRESERVE_BYTES[];

};

#endif // QNUTILS_H
