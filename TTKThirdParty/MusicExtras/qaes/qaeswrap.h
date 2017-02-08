#ifndef QAESWRAP_H
#define QAESWRAP_H

extern "C" {
#include "aes.h"
}
#include <QByteArray>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QAesWrapPrivate;

/*! @brief The namespace of the aes wrapper.
 * @author dushibaiyu <dushibaiyu@yahoo.com>
 */
class MUSIC_EXTRAS_EXPORT QAesWrap
{
public:
    enum AesBit {
        AES_128 = 128,
        AES_192 = 192,
        AES_256 = 256
    };
    enum AesMode {
        AES_ECB,
        AES_CBC,
        AES_CTR
    };
    enum PaddingMode {
        None,
        ANSIX923,
        PKCS7
    };
public:
    QAesWrap(const QByteArray &passwprd, const QByteArray &salt, AesBit bit);
    /*!
     * Object contsructor.
     */

    bool encrypt(const QByteArray &in, QByteArray &out, AesMode mode, PaddingMode pad = PKCS7) const;
    /*!
     * Encrypt aes by input.
     */
    bool decrypt(const QByteArray &in, QByteArray &out, AesMode mode, PaddingMode pad = PKCS7) const;
    /*!
     * Decrypt aes by input.
     */

    inline QByteArray encrypt(const QByteArray &data, AesMode mode, PaddingMode pad = PKCS7) const
    {
          QByteArray out;
          encrypt(data,out,mode,pad);
          return out;
    }
    /*!
     * Encrypt aes by input.
     */
    inline QByteArray decrypt(const QByteArray &data, AesMode mode, PaddingMode pad = PKCS7) const
    {
          QByteArray out;
          decrypt(data,out,mode,pad);
          return out;
    }
    /*!
     * Decrypt aes by input.
     */

private:
    TTK_DECLARE_PRIVATE(QAesWrap)

};

#endif // QAESWRAP_H
