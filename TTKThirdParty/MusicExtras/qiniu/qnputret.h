#ifndef QNPUTRET_H
#define QNPUTRET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNPutRetPrivate;

class MUSIC_EXTRAS_EXPORT QNPutRet
{
public:
    QNPutRet();

    static QNPutRet* fromJSON(const QByteArray &json);

    QString getHash() const;
    void setHash(const QString &value);

    QString getKey() const;
    void setKey(const QString &value);

private:
    TTK_DECLARE_PRIVATE(QNPutRet)

};

#endif // QNPUTRET_H
