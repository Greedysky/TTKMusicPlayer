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

/*! @brief The namespace of the qiniu put extra.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNPutExtra
{
public:
    QNPutExtra();
    /*!
     * Object contsructor.
     */

    void addExtraParam(const QString &key, const QString &value);
    /*!
     * Add extra parameter.
     */
    QString removeExtraParam(const QString &key);
    /*!
     * Reomove extra parameter.
     */

    QString getMimeType() const;
    /*!
     * Get mime type.
     */
    void setMimeType(const QString &value);
    /*!
     * Set mime type.
     */

    qint32 getCrc32() const;
    /*!
     * Get crc32.
     */
    void setCrc32(qint32 value);
    /*!
     * Set crc32.
     */

    qint32 getCheckCrc32() const;
    /*!
     * Get check crc32.
     */
    void setCheckCrc32(qint32 value);
    /*!
     * Set check crc32.
     */

private:
    TTK_DECLARE_PRIVATE(QNPutExtra)

};

#endif // QNPUTEXTRA_H
