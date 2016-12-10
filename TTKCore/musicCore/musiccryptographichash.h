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

#define DOWNLOAD_KEY "5024FRSDF40"
#define URL_KEY      "GREEDYSKY"

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
        Lower = 1,  ///*Priority Lower*/
        Low,        ///*Priority Low*/
        Normal,     ///*Priority Normal*/
        High,       ///*Priority High*/
        Higher      ///*Priority Higher*/
    };

    MusicCryptographicHash();
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    QString encrypt(const QString &data, const QString &key, Priority p = Lower);
    /*!
     * encrypt by QString data.
     */
    QString decrypt(const QString &data, const QString &key, Priority p = Lower);
    /*!
     * decrypt by QString data.
     */
    static QString encryptData(const QString &data, const QString &key, Priority p = Lower);
    /*!
     * encrypt by QString data.
     */
    static QString decryptData(const QString &data, const QString &key, Priority p = Lower);
    /*!
     * decrypt by QString data.
     */

    //////////////////////////////////////////////////////////////////////////
    std::string xxteaEncrypt(std::string data,  std::string key);
    /*!
     * XXTEA encrypt by std::string data.
     */
    QString xxteaEncrypt(const QString &data, const QString &key);
    /*!
     * XXTEA encrypt by QString data.
     */

    std::string xxteaDecrypt(std::string data,  std::string key);
    /*!
     * XXTEA decrypt by std::string data.
     */
    QString xxteaDecrypt(const QString &data, const QString &key);
    /*!
     * XXTEA decrypt by QString data.
     */

protected:
    bool isBase64(unsigned char c);
    /*!
     * Check current char is base64.
     */
    unsigned char *doXxteaEncrypt(unsigned char *data, xxtea_uint len, unsigned char *key, xxtea_uint *ret_len);
    /*!
     * Do XXTEA encrypt.
     */
    unsigned char *doXxteaDecrypt(unsigned char *data, xxtea_uint len, unsigned char *key, xxtea_uint *ret_len);
    /*!
     * Do XXTEA decrypt.
     */
    void xxteaUintEncrypt(xxtea_uint *v, xxtea_uint len, xxtea_uint *k);
    /*!
     * XXTEA uint encrypt.
     */
    void xxteaUintDecrypt(xxtea_uint *v, xxtea_uint len, xxtea_uint *k);
    /*!
     * XXTEA uint decrypt.
     */
    unsigned char *fixKeyLength(unsigned char *key, xxtea_uint key_len);
    /*!
     * Fix key length.
     */
    xxtea_uint *xxteaToUintArray(unsigned char *data, xxtea_uint len, int include_length, xxtea_uint *ret_len);
    /*!
     * XXTEA to uint array.
     */
    unsigned char *xxteaToByteArray(xxtea_uint *data, xxtea_uint len, int include_length, xxtea_uint *ret_len);
    /*!
     * XXTEA to byte array.
     */
    //////////////////////////////////////////////////////////////////////////
    std::string base64Encode(unsigned char const* , unsigned int len);
    /*!
     * Base64 encode.
     */
    std::string base64Decode(std::string const& s);
    /*!
     * Base64 dncode.
     */
    //////////////////////////////////////////////////////////////////////////
    unsigned char *xxteaEncrypt(unsigned char *data, xxtea_uint data_len, unsigned char *key, xxtea_uint key_len, xxtea_uint *ret_length);
    /*!
     * XXTEA encrypt by unsigned char * data.
     */
    unsigned char *xxteaDecrypt(unsigned char *data, xxtea_uint data_len, unsigned char *key, xxtea_uint key_len, xxtea_uint *ret_length);
    /*!
     * XXTEA decrypt by unsigned char * data.
     */
    //////////////////////////////////////////////////////////////////////////

};

#endif // MUSICCRYPTOGRAPHICHASH_H
