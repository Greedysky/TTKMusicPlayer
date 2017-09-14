#ifndef MUSICCRYPTOGRAPHICHASH_H
#define MUSICCRYPTOGRAPHICHASH_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QString>
#include "musicglobaldefine.h"

#ifdef Q_CC_GNU
    #pragma GCC diagnostic ignored "-Wparentheses"
#endif

#if defined(_MSC_VER)
typedef unsigned __int32 xxtea_uint;
#pragma warning(disable:4267)
#else
# if defined(__FreeBSD__) && __FreeBSD__ < 5
#   include <inttypes.h>
# else
#   include <stdint.h>
#  endif
typedef uint32_t xxtea_uint;
#endif

/*! @brief The class of the string cryptographic hash.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicCryptographicHash
{
public:
    enum Priority
    {
        Lower = 1,  /*!< Priority Lower*/
        Low,        /*!< Priority Low*/
        Normal,     /*!< Priority Normal*/
        High,       /*!< Priority High*/
        Higher      /*!< Priority Higher*/
    };

    /*!
     * Object contsructor.
     */
    MusicCryptographicHash();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * encrypt by QString data.
     */
    QString encrypt(const QString &data, const QString &key, Priority p = Lower);
    /*!
     * decrypt by QString data.
     */
    QString decrypt(const QString &data, const QString &key, Priority p = Lower);

    /*!
     * XXTEA encrypt by std::string data.
     */
    std::string xxteaEncrypt(std::string data,  std::string key);
    /*!
     * XXTEA encrypt by QString data.
     */
    QString xxteaEncrypt(const QString &data, const QString &key);

    /*!
     * XXTEA decrypt by std::string data.
     */
    std::string xxteaDecrypt(std::string data,  std::string key);
    /*!
     * XXTEA decrypt by QString data.
     */
    QString xxteaDecrypt(const QString &data, const QString &key);

protected:
    /*!
     * Check current char is base64.
     */
    bool isBase64(unsigned char c);
    /*!
     * Do XXTEA encrypt.
     */
    unsigned char *doXxteaEncrypt(unsigned char *data, xxtea_uint len, unsigned char *key, xxtea_uint *ret_len);
    /*!
     * Do XXTEA decrypt.
     */
    unsigned char *doXxteaDecrypt(unsigned char *data, xxtea_uint len, unsigned char *key, xxtea_uint *ret_len);
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
    unsigned char *fixKeyLength(unsigned char *key, xxtea_uint key_len);
    /*!
     * XXTEA to uint array.
     */
    xxtea_uint *xxteaToUintArray(unsigned char *data, xxtea_uint len, int include_length, xxtea_uint *ret_len);
    /*!
     * XXTEA to byte array.
     */
    unsigned char *xxteaToByteArray(xxtea_uint *data, xxtea_uint len, int include_length, xxtea_uint *ret_len);

    //////////////////////////////////////////////////////////////////////////
    /*!
     * Base64 encode.
     */
    std::string base64Encode(unsigned char const* , unsigned int len);
    /*!
     * Base64 dncode.
     */
    std::string base64Decode(std::string const& s);

    //////////////////////////////////////////////////////////////////////////
    /*!
     * XXTEA encrypt by unsigned char * data.
     */
    unsigned char *xxteaEncrypt(unsigned char *data, xxtea_uint data_len, unsigned char *key, xxtea_uint key_len, xxtea_uint *ret_length);
    /*!
     * XXTEA decrypt by unsigned char * data.
     */
    unsigned char *xxteaDecrypt(unsigned char *data, xxtea_uint data_len, unsigned char *key, xxtea_uint key_len, xxtea_uint *ret_length);
    //////////////////////////////////////////////////////////////////////////

};

#endif // MUSICCRYPTOGRAPHICHASH_H
