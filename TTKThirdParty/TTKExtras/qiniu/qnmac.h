#ifndef QNMAC_H
#define QNMAC_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QtNetwork/QNetworkRequest>
#include "musicprivate.h"
#include "musicextrasglobaldefine.h"

class QNMacPrivate;

/*! @brief The namespace of the qiniu mac.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNMac
{
public:
    /*!
     * Object contsructor.
     */
    QNMac(const QString &accessKey, const QByteArray &secretKey);

    /*!
     * Qiniu to sign.
     */
    QString sign(const QByteArray &data) const;
    /*!
     * Qiniu to sign with data.
     */
    QString signWithData(const QByteArray &data) const;
    /*!
     * Qiniu to sign with data2.
     */
    QString signWithData2(const QByteArray &data) const;
    /*!
     * Qiniu to sign request.
     */
    QString signRequest(const QUrl &reqestUrl, const QByteArray &bodyData = 0) const;
    /*!
     * Qiniu to sign request.
     */
    QString signRequest(const QNetworkRequest &request, const QByteArray &bodyData = 0) const;

private:
    TTK_DECLARE_PRIVATE(QNMac)

};

#endif // QNMAC_H
