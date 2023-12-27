#ifndef QDLNAFILESERVER_H
#define QDLNAFILESERVER_H

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

#include "qdlnahelper.h"
#include "ttkprivate.h"

class QHttpRequest;
class QHttpResponse;
class QDlnaFileServerPrivate;

/*! @brief The class of the dlna file server.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QDlnaFileServer : public QObject
{
    Q_OBJECT
public:
    explicit QDlnaFileServer(QObject *parent = nullptr);

    void start();

    void setPrefixPath(const QString &path);
    QString localAddress(const QString &prefix) const;

private Q_SLOTS:
    void handleRequest(QHttpRequest *request, QHttpResponse *response);

private:
    TTK_DECLARE_PRIVATE(QDlnaFileServer)

};

#endif // QDLNAFILESERVER_H
