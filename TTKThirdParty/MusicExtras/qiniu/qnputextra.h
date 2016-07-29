#ifndef QNPUTEXTRA_H
#define QNPUTEXTRA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QMap>
#include <QString>
#include "musicextrasglobaldefine.h"

class MUSIC_EXTRAS_EXPORT QNPutExtra
{
public:
    static const int NO_CRC32 = 0;
    static const int AUTO_CRC32 = 1;
    static const int WITH_CRC32 = 2;

    QNPutExtra();
    ~QNPutExtra();

    void addExtraParam(const QString &key, const QString &value);
    QString removeExtraParam(const QString &key);

    QString getMimeType() const;
    void setMimeType(const QString &value);

    qint32 getCrc32() const;
    void setCrc32(qint32 value);

    qint32 getCheckCrc32() const;
    void setCheckCrc32(qint32 value);

private:
    QMap<QString, QString> m_params;
    QString m_mimeType;
    qint32 m_crc32, m_checkCrc32;

};

#endif // QNPUTEXTRA_H
