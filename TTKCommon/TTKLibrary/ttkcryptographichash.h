#ifndef TTKCRYPTOGRAPHICHASH_H
#define TTKCRYPTOGRAPHICHASH_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2025 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include "ttkmoduleexport.h"

#if defined(_MSC_VER)
using xxtea_uint = unsigned __int32;
#  pragma warning(disable:4267)
#else
#  if defined(__FreeBSD__) && __FreeBSD__ < 5
#    include <inttypes.h>
#  else
#    include <stdint.h>
#  endif
using xxtea_uint = uint32_t;
#endif

/*! @brief The class of the string cryptographic hash.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKCryptographicHash
{
public:
    /*!
     * encrypt by QString data.
     */
    QString encrypt(const QString &data, const char *key);
    /*!
     * decrypt by QString data.
     */
    QString decrypt(const QString &data, const char *key);

private:
    /*!
     * XXTEA encrypt by TTKString data.
     */
    TTKString xxteaEncrypt(const TTKString &data, const char *key);
    /*!
     * XXTEA decrypt by TTKString data.
     */
    TTKString xxteaDecrypt(const TTKString &data, const char *key);

    /*!
     * Do XXTEA encrypt.
     */
    uchar *doXxteaEncrypt(uchar *data, xxtea_uint len, uchar *key, xxtea_uint *retLength);
    /*!
     * Do XXTEA decrypt.
     */
    uchar *doXxteaDecrypt(uchar *data, xxtea_uint len, uchar *key, xxtea_uint *retLength);
    /*!
     * XXTEA uint encrypt.
     */
    void xxteaUintEncrypt(xxtea_uint *v, xxtea_uint len, xxtea_uint *k);
    /*!
     * XXTEA uint decrypt.
     */
    void xxteaUintDecrypt(xxtea_uint *v, xxtea_uint len, xxtea_uint *k);
    /*!
     * Fix key length.
     */
    uchar *fixKeyLength(uchar *key, xxtea_uint keyLength);
    /*!
     * XXTEA to uint array.
     */
    xxtea_uint *xxteaToUintArray(uchar *data, xxtea_uint len, int includeLength, xxtea_uint *retLength);
    /*!
     * XXTEA to byte array.
     */
    uchar *xxteaToByteArray(xxtea_uint *data, xxtea_uint len, int includeLength, xxtea_uint *retLength);

    /*!
     * XXTEA encrypt by uchar * data.
     */
    uchar *xxteaEncrypt(uchar *data, xxtea_uint dataLength, uchar *key, xxtea_uint keyLength, xxtea_uint *retLengthgth);
    /*!
     * XXTEA decrypt by uchar * data.
     */
    uchar *xxteaDecrypt(uchar *data, xxtea_uint dataLength, uchar *key, xxtea_uint keyLength, xxtea_uint *retLengthgth);

};


/*! @brief The namespace of the base64 helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Base64 encode.
     */
    TTK_MODULE_EXPORT TTKString base64Encode(const unsigned char *bytes, unsigned int len);
    /*!
     * Base64 dncode.
     */
    TTK_MODULE_EXPORT TTKString base64Decode(const TTKString &bytes);

}

#endif // TTKCRYPTOGRAPHICHASH_H
