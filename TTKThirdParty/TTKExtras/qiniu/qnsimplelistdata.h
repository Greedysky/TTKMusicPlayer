#ifndef QNSIMPLELISTDATA_H
#define QNSIMPLELISTDATA_H

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

#include "ttkprivate.h"
#include "qndataitem.h"
#include <QtNetwork/QNetworkReply>

class QNSimpleListDataPrivate;

/*! @brief The namespace of the qiniu simple list data.
 * @author Jemy Graw <jemygraw@gmail.com>
 */
class MUSIC_EXTRAS_EXPORT QNSimpleListData : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit QNSimpleListData(QNetworkAccessManager *networkManager, QObject *parent = nullptr);
    /*!
     * List data to server.
     */
    void listDataToServer(const QString &bucket);

Q_SIGNALS:
    /*!
     * Receive data finshed.
     */
    void receiveFinshed(const QNDataItems &items);

private Q_SLOTS:
    /*!
     * Receive data from server finshed.
     */
    void receiveDataFromServer();
    /*!
     * Get handle error.
     */
    void handleError(QNetworkReply::NetworkError error);

private:
    TTK_DECLARE_PRIVATE(QNSimpleListData)

};

#endif // QNSIMPLELISTDATA_H
