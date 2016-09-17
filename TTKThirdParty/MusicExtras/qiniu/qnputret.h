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

/*! @brief The namespace of the qiniu put return.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNPutRet
{
public:
    QNPutRet();
    /*!
     * Object contsructor.
     */

    static QNPutRet* fromJSON(const QByteArray &json);
    /*!
     * Transfer data from json.
     */

    QString getHash() const;
    /*!
     * Get hash.
     */
    void setHash(const QString &value);
    /*!
     * Set hash.
     */

    QString getKey() const;
    /*!
     * Get key.
     */
    void setKey(const QString &value);
    /*!
     * Set key.
     */

private:
    TTK_DECLARE_PRIVATE(QNPutRet)

};

#endif // QNPUTRET_H
