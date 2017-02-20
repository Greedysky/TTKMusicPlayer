#ifndef QNIOHELPER_H
#define QNIOHELPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "qnconf.h"
#include <QHttpMultiPart>

class QNMac;
class QNPutExtra;

/*! @brief The namespace of the qiniu io helper.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNIOHelper
{

public:
    static QHttpMultiPart* createPutDataMultiPart(const QString &uploadToken, const QByteArray &data,
                                                  const QString &key = 0, const QString &fileName = 0,
                                                  const QNPutExtra *putExtra = 0);
    /*!
     * Create put data multi part.
     */
    static QNetworkRequest listRequest(const QString &bucket, const QNMac *mac = 0);
    /*!
     * List request.
     */
    static QNetworkRequest deleteRequest(const QString &bucket, const QString &key, const QNMac *mac = 0);
    /*!
     * Delete requestt.
     */

};

#endif // QNIO_H
