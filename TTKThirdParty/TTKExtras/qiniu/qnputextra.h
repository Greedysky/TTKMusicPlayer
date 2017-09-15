#ifndef QNPUTEXTRA_H
#define QNPUTEXTRA_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QString>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNPutExtraPrivate;

/*! @brief The namespace of the qiniu put extra.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNPutExtra
{
public:
    /*!
     * Object contsructor.
     */
    QNPutExtra();

    /*!
     * Add extra parameter.
     */
    void addExtraParam(const QString &key, const QString &value);
    /*!
     * Reomove extra parameter.
     */
    QString removeExtraParam(const QString &key);

    /*!
     * Get mime type.
     */
    QString getMimeType() const;
    /*!
     * Set mime type.
     */
    void setMimeType(const QString &value);

    /*!
     * Get crc32.
     */
    qint32 getCrc32() const;
    /*!
     * Set crc32.
     */
    void setCrc32(qint32 value);

    /*!
     * Get check crc32.
     */
    qint32 getCheckCrc32() const;
    /*!
     * Set check crc32.
     */
    void setCheckCrc32(qint32 value);

private:
    MUSIC_DECLARE_PRIVATE(QNPutExtra)

};

#endif // QNPUTEXTRA_H
