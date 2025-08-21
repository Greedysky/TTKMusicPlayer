#ifndef MUSICABSTRACTNETWORK_H
#define MUSICABSTRACTNETWORK_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "ttkabstractnetwork.h"
#include "musicnetworkthread.h"
#include "musicnetworkdefines.h"
#include "musicnumberutils.h"
#include "musicstringutils.h"
#include "musicalgorithmutils.h"
#include "musicsettingmanager.h"

#include "qjson/json.h"

/*! @brief The class of the abstract network.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractNetwork : public TTKAbstractNetwork
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractNetwork(QObject *parent = nullptr);

public Q_SLOTS:
    /*!
     * Download reply error.
     */
    virtual void replyError(QNetworkReply::NetworkError error) override;
#ifndef QT_NO_SSL
    /*!
     * Download ssl reply error.
     */
    virtual void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors) override;
#endif

};

#endif // MUSICABSTRACTNETWORK_H
