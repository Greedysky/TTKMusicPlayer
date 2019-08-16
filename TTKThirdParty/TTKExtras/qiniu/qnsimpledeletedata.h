#ifndef QNSIMPLEDELETEDATA_H
#define QNSIMPLEDELETEDATA_H

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

#include <QtNetwork/QNetworkReply>
#include "ttkprivate.h"
#include "musicextrasglobaldefine.h"

class QNSimpleDeleteDataPrivate;

/*! @brief The namespace of the qiniu simple delete data.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNSimpleDeleteData : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit QNSimpleDeleteData(QNetworkAccessManager *networkManager, QObject *parent = nullptr);
    /*!
     * Delete data to server.
     */
    void deleteDataToServer(const QString &bucket, const QString &key);

Q_SIGNALS:
    /*!
     * Delete file finished.
     */
    void deleteFileFinished(bool state);

private Q_SLOTS:
    /*!
     * Receive data from server.
     */
    void receiveDataFromServer();
    /*!
     * Get handle error.
     */
    void handleError(QNetworkReply::NetworkError error);

private:
    TTK_DECLARE_PRIVATE(QNSimpleDeleteData)

};

#endif // QNSIMPLEDELETEDATA_H
