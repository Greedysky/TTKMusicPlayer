#ifndef QSYNCDELETEDATA_H
#define QSYNCDELETEDATA_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include "qsyncdatainterface.h"

/*! @brief The class of the sync cloud delete data.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QSyncDeleteData : public QSyncDataInterface
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit QSyncDeleteData(QNetworkAccessManager *networkManager, QObject *parent = nullptr);
    
    /*!
     * Delete data operater.
     */
    void request(const QString &bucket, const QString &fileName);

Q_SIGNALS:
    /*!
     * Delete file finished.
     */
    void deleteFileFinished(bool state);

private Q_SLOTS:
    /*!
     * Receive data from server.
     */
    virtual void receiveDataFromServer() override final;

};

#endif // QSYNCDELETEDATA_H
