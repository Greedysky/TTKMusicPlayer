#ifndef TTKLOCALPEER_H
#define TTKLOCALPEER_H

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

#include <QObject>
#include "ttkprivate.h"
#include "musicrunglobaldefine.h"

class TTKLocalPeerPrivate;

/*! @brief The class of the ttk local peer.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_RUN_EXPORT TTKLocalPeer : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
    */
    explicit TTKLocalPeer(QObject *parent = nullptr, const QString &appId = QString());

    /*!
     * Current client is running or not.
    */
    bool isClient();

    /*!
     * Send current message when the client in.
    */
    bool sendMessage(const QString &message, int timeout);

    /*!
     * Get current server id.
    */
    QString applicationId() const;

Q_SIGNALS:
    /*!
     * Emit when the current message received.
    */
    void messageReceived(const QString &message);

protected Q_SLOTS:
    /*!
     * Current message received.
    */
    void receiveConnection();

private:
    TTK_DECLARE_PRIVATE(TTKLocalPeer)

};

#endif // TTKLOCALPEER_H
