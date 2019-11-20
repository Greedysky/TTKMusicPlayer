#ifndef OSSDATAINTERFACE_H
#define OSSDATAINTERFACE_H

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

#include "ossdataitem.h"
#include "ossutils.h"
#include "ossconf.h"
#include <QtNetwork/QNetworkReply>

class OSSDataInterfacePrivate;

/*! @brief The class of the alioss cloud data item.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT OSSDataInterface : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit OSSDataInterface(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

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
    TTK_DECLARE_PRIVATE(OSSDataInterface)

};

#endif // OSSDATAINTERFACE_H
