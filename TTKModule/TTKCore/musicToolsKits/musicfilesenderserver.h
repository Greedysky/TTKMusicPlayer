#ifndef MUSICFILESENDERSERVER_H
#define MUSICFILESENDERSERVER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include <QFile>
#include <QUdpSocket>
#include <QStringList>
#include "musicglobaldefine.h"

#define MAX_DATA        8000
#define SEND_PORT       7754
#define RECEVIE_PORT    7755
#define DATAGRAM        "BACK_ON"

/*! @brief The class of the file sender server.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicFileSenderServer : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFileSenderServer(QObject *parent = 0);

    ~MusicFileSenderServer();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to transfer file.
     */
    void start();
    /*!
     * Set send file.
     */
    void setSendFile(const QString &file);
    /*!
     * Set send file list.
     */
    void setSendFiles(const QStringList &files);
    /*!
     * Set receive address.
     */
    void setReceiveAddress(const QString &ip);
    /*!
     * Get receive address.
     */
    QString getReceiveAddress() const;

Q_SIGNALS:
    /*!
     * Send file just has finished.
     */
    void fileSendFinished();

private Q_SLOTS:
    /*!
     * Read pending datagrams.
     */
    void readPendingDatagrams();

protected:
    /*!
     * Send file data.
     */
    void sendData();

    bool m_nextFile;
    QUdpSocket *m_sendSocket;
    QFile *m_file;
    QStringList m_files;
    QString m_receiveIp;

};

#endif // MUSICFILESENDERSERVER_H
