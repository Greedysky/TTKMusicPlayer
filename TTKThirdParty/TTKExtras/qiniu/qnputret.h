#ifndef QNPUTRET_H
#define QNPUTRET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNPutRetPrivate;

/*! @brief The namespace of the qiniu put return.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNPutRet
{
public:
    /*!
     * Object contsructor.
     */
    QNPutRet();

    /*!
     * Transfer data from json.
     */
    static QNPutRet* fromJSON(const QByteArray &json);

    /*!
     * Get hash.
     */
    QString getHash() const;
    /*!
     * Set hash.
     */
    void setHash(const QString &value);

    /*!
     * Get key.
     */
    QString getKey() const;
    /*!
     * Set key.
     */
    void setKey(const QString &value);

private:
    MUSIC_DECLARE_PRIVATE(QNPutRet)

};

#endif // QNPUTRET_H
