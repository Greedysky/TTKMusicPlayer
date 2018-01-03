#ifndef QDESWRAP_H
#define QDESWRAP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2018 Greedysky Studio

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

#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the des wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QDesWrap
{
public:
    enum Mode
    {
        ENCRYPT = 0,
        DECRYPT = 1
    };

    /*!
     * Encrypt des by input.
     */
    QByteArray encrypt(const QByteArray &in, const QByteArray &key);
    /*!
     * Decrypt des by input.
     */
    QByteArray decrypt(const QByteArray &in, const QByteArray &key);

protected:
    qint64 bitTransform(int *array, int len, qint64 source);
    void DESSubKeys(qint64 key, qint64* K, Mode mode);
    qint64 DES64(qint64 *subkeys, qint64 data);
    char* encrypt(char* src, int srcLength, char* key);

    Mode m_mode;
};

#endif // QDESWRAP_H
