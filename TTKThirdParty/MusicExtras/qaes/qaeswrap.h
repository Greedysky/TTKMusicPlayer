#ifndef QAESWRAP_H
#define QAESWRAP_H

#include "musicextrasglobaldefine.h"

/*! @brief The namespace of the aes wrapper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QAesWrap
{
public:
    static QByteArray encrypt(const QString &in, const QString &key, const QString &iv);
    /*!
     * Encrypt aes by input.
     */
    static QByteArray decrypt(const QByteArray &in, const QString &key, const QString &iv);
    /*!
     * Decrypt aes by input.
     */

};

#endif // QAESWRAP_H
