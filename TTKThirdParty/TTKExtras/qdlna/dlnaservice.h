#ifndef DLNASERVICE_H
#define DLNASERVICE_H

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

/*! @brief The class of the dlna service.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_EXTRAS_EXPORT DLNAService
{
public:
    DLNAService();
    DLNAService(const QString &control, const QString &scpd, const QString &eventSub, const QString &type, const QString &id);

    QString getServiceID() const;
    QString getControlURL() const;

private:
    QString m_controlURL;
    QString m_scpdurl;
    QString m_eventSubURL;
    QString m_serviceType;
    QString m_serviceID;

};

#endif // DLNASERVICE_H
