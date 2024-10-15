#ifndef QDLNACLIENT_H
#define QDLNACLIENT_H

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

class QDlnaClientPrivate;

/*! @brief The class of the dlna client.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT QDlnaClient
{
public:
    explicit QDlnaClient(const QString &data);

    QString server() const;
    QString serverName() const;

    bool connect() const;
    bool isConnected() const;

    bool open(const QString &url) const;

    bool play(int instance = 0) const;
    bool pause(int instance = 0) const;
    bool stop(int instance = 0) const;
    bool remove(int instance = 0) const;

    bool position(qint64 &position, qint64 &duration, int instance = 0) const;

private:
    TTK_DECLARE_PRIVATE(QDlnaClient)

};

#endif // QDLNACLIENT_H
