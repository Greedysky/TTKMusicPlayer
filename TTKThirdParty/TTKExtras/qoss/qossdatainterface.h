#ifndef QOSSDATAINTERFACE_H
#define QOSSDATAINTERFACE_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include "qossdataitem.h"
#include "qossutils.h"
#include "qossconf.h"
#include <QtNetwork/QNetworkReply>

class QOSSDataInterfacePrivate;

/*! @brief The class of the alioss cloud data item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT QOSSDataInterface : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit QOSSDataInterface(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

protected Q_SLOTS:
    /*!
     * Receive data from server finshed.
     * Subclass should implement this function.
     */
    virtual void receiveDataFromServer() = 0;
    /*!
     * Get handle error.
     */
    void handleError(QNetworkReply::NetworkError error);

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
    TTK_DECLARE_PRIVATE(QOSSDataInterface)

};

#endif // QOSSDATAINTERFACE_H
