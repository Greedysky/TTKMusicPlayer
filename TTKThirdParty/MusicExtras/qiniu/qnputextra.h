#ifndef QNPUTEXTRA_H
#define QNPUTEXTRA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QString>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNPutExtraPrivate;

class MUSIC_EXTRAS_EXPORT QNPutExtra
{
public:
    QNPutExtra();

    void addExtraParam(const QString &key, const QString &value);
    QString removeExtraParam(const QString &key);

    QString getMimeType() const;
    void setMimeType(const QString &value);

    qint32 getCrc32() const;
    void setCrc32(qint32 value);

    qint32 getCheckCrc32() const;
    void setCheckCrc32(qint32 value);

private:
    TTK_DECLARE_PRIVATE(QNPutExtra)

};

#endif // QNPUTEXTRA_H
