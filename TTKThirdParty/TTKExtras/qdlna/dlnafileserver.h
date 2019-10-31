#ifndef DLNAFILESERVER_H
#define DLNAFILESERVER_H

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

#include "dlnahelper.h"
#include "musicextrasglobaldefine.h"

class QHttpServer;
class QHttpRequest;
class QHttpResponse;

/*! @brief The class of the dlna file server.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT DlnaFileServer : public QObject
{
    Q_OBJECT
public:
    DlnaFileServer(QObject *parent = nullptr);
    ~DlnaFileServer();

    void start();
    void setPrefixPath(const QString &path);
    QString getLocalAddress() const;

private Q_SLOTS:
    void handleRequest(QHttpRequest *request, QHttpResponse *response);

private:
    QString m_prefix;
    QHttpServer *m_server;

};

#endif // DLNAFILESERVER_H
