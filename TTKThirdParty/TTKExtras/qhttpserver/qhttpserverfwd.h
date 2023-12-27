#ifndef QHTTPSERVERFWD_H
#define QHTTPSERVERFWD_H

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

#include "ttkprivate.h"

/*!
 * A map of request or response headers
 */
typedef QHash<QString, QString> HeaderHash;

// QHttpServer
class QHttpServer;
class QHttpConnection;
class QHttpRequest;
class QHttpResponse;

// Qt
class QTcpServer;
class QTcpSocket;

// http_parser
struct http_parser_settings;
struct http_parser;

#endif
