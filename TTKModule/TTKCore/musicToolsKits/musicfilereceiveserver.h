#ifndef MUSICFILERECEIVESERVER_H
#define MUSICFILERECEIVESERVER_H

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

#include "musicfilesenderserver.h"

/*! @brief The class of the file receive server.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicFileReceiveServer : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFileReceiveServer)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFileReceiveServer(QObject *parent = nullptr);

    ~MusicFileReceiveServer();

    /*!
     * Set save file dir.
     */
    void setSavePathDir(const QString &dir);

private Q_SLOTS:
    /*!
     * Read pending datagrams.
     */
    void readPendingDatagrams();

protected:
    QFile *m_file;
    QUdpSocket *m_receiveSocket;
    QString m_saveDir;

};

#endif // MUSICFILERECEIVESERVER_H
