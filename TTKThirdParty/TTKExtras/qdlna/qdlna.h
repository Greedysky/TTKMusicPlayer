#ifndef QDLNA_H
#define QDLNA_H

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

#include "ttkprivate.h"

/*! @brief The namespace of the dlna helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace QDlna
{
    /*!
     * Make socket get reply.
     */
    QString makeSocketGetReply(const QString &ip, const QString &port, const QString &data);
    /*!
     * Make request.
     */
    QString makeRequest(const QString &methord, const QString &url, int length, const QString &soapAction, const QString &ip, const QString &port);
    /*!
     * Remove http header.
     */
    QString removeHttpHeader(const QString &data);

    /*!
     * Get response code.
     */
    bool isValid(const QString &data);
    /*!
     * Get response code.
     */
    int getResponseCode(const QString &data);

}

#endif // QDLNA_H
