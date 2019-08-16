#ifndef QNPUTEXTRA_H
#define QNPUTEXTRA_H

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

#include <QString>
#include "ttkprivate.h"
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
    TTK_DECLARE_PRIVATE(QNPutExtra)

};

#endif // QNPUTEXTRA_H
