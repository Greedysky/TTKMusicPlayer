#ifndef QDLNACLIENT_H
#define QDLNACLIENT_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

namespace QDlna {
struct PositionInfo;
struct TransportInfo;
struct MediaInfo;
}

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

public:
    bool openUri(const QString &url, const QString &name, int instance = 0) const;
    bool openUris(const QString &url, const QString &name, int instance = 0) const;
    bool nextUri(const QString &url, const QString &name, int instance = 0) const;

    bool positionInfo(QDlna::PositionInfo &info, int instance = 0) const;
    bool transportInfo(QDlna::TransportInfo &info, int instance = 0) const;
    bool mediaInfo(QDlna::MediaInfo &info, int instance = 0) const;

    bool play(int instance = 0) const;
    bool pause(int instance = 0) const;
    bool stop(int instance = 0) const;
    bool remove(int instance = 0) const;
    bool previous(int instance = 0) const;
    bool next(int instance = 0) const;
    bool setPlayMode(const QString &mode, int instance) const;

private:
    TTK_DECLARE_PRIVATE(QDlnaClient)

};

#endif // QDLNACLIENT_H
