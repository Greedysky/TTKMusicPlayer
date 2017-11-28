#ifndef QAESWRAP_H
#define QAESWRAP_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

extern "C" {
#include "aes.h"
}
#include <QByteArray>
#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the aes wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QAesWrap
{
public:
    enum AesBit
    {
        AES_128 = 128,
        AES_192 = 192,
        AES_256 = 256
    };

    enum AesMode
    {
        AES_ECB,
        AES_CBC,
        AES_CTR
    };

    enum PaddingMode
    {
        None,
        ANSIX923,
        PKCS7
    };

    QAesWrap(const QByteArray &passwprd, const QByteArray &salt, AesBit bit);

    bool encrypt(const QByteArray &in, QByteArray &out, AesMode mode, PaddingMode pad = PKCS7) const;
    bool decrypt(const QByteArray &in, QByteArray &out, AesMode mode, PaddingMode pad = PKCS7) const;

    inline QByteArray encrypt(const QByteArray &data, AesMode mode,PaddingMode pad = PKCS7) const
    {
          QByteArray out;
          encrypt(data, out, mode, pad);
          return out.toBase64();
    }

    inline QByteArray decrypt(const QByteArray &data, AesMode mode,PaddingMode pad = PKCS7) const
    {
          QByteArray out;
          decrypt(QByteArray::fromBase64(data), out, mode, pad);
          return out;
    }

private:
    void ecbencrypt(const BYTE *in, size_t size, BYTE *blcok, QByteArray &out) const;
    void ecbdecrypt(const BYTE *in, size_t size, QByteArray & out) const;
    void initPadding(const QByteArray &in, QByteArray &out, AesMode mode, PaddingMode pad) const;

    AesBit mbit;
    WORD mpass[60];
    BYTE msalt[AES_BLOCK_SIZE];
};

#endif // QAESWRAP_H
