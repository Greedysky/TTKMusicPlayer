#ifndef QDLNASERVICE_H
#define QDLNASERVICE_H

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

#include "qdlna.h"

/*! @brief The class of the dlna service.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT QDlnaService
{
    QString m_controlURL;
    QString m_scpdURL;
    QString m_eventSubURL;
    QString m_serviceType;
    QString m_serviceID;

    inline bool isEmpty() const
    {
        return m_controlURL.isEmpty() && m_scpdURL.isEmpty() && m_eventSubURL.isEmpty() && m_serviceType.isEmpty() && m_serviceID.isEmpty();
    }
};

#endif // QDLNASERVICE_H
