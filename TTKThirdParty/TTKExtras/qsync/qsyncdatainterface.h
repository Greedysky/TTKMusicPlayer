#ifndef QSYNCDATAINTERFACE_H
#define QSYNCDATAINTERFACE_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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
 ***************************************************************************/

#include "qsyncdataitem.h"
#include "qsyncutils.h"
#include "qsyncconfig.h"
#include <QtNetwork/QNetworkReply>

class QSyncDataInterfacePrivate;

/*! @brief The class of the sync cloud data interface.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QSyncDataInterface : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit QSyncDataInterface(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

protected Q_SLOTS:
    /*!
     * Receive data from server finshed.
     * Subclass should implement this function.
     */
    virtual void receiveDataFromServer() = 0;
    /*!
     * Get handle error.
     */
    void replyError(QNetworkReply::NetworkError error);

protected:
    /*!
     * Pretty encode the data into url.
     */
    QString pathEncode(const QString &data) const;
    /*!
     * Pretty decode the data into url.
     */
    QString pathDecode(const QString &data) const;

protected:
    /*!
     * Object contsructor.
     */
    QSyncDataInterface(QSyncDataInterfacePrivate &pvt, QNetworkAccessManager *networkManager, QObject *parent = nullptr);

protected:
    TTK_DECLARE_PRIVATE(QSyncDataInterface)

};

#endif
