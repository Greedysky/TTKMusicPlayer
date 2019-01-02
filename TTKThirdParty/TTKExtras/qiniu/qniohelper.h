#ifndef QNIOHELPER_H
#define QNIOHELPER_H

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

#include "qnconf.h"
#include <QtNetwork/QHttpMultiPart>

class QNMac;
class QNPutExtra;

/*! @brief The namespace of the qiniu io helper.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNIOHelper
{

public:
    /*!
     * Create put data multi part.
     */
    static QHttpMultiPart* createPutDataMultiPart(const QString &uploadToken, const QByteArray &data, const QString &key = QString(), const QString &fileName = QString(), const QNPutExtra *putExtra = nullptr);
    /*!
     * List request.
     */
    static QNetworkRequest listRequest(const QString &bucket, const QNMac *mac = nullptr);
    /*!
     * Delete requestt.
     */
    static QNetworkRequest deleteRequest(const QString &bucket, const QString &key, const QNMac *mac = nullptr);

};

#endif // QNIO_H
