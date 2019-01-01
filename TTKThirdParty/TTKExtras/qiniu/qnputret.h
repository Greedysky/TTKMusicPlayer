#ifndef QNPUTRET_H
#define QNPUTRET_H

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

#include <QObject>
#include "ttkprivate.h"
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
    TTK_DECLARE_PRIVATE(QNPutRet)

};

#endif // QNPUTRET_H
