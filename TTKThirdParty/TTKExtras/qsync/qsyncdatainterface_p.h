#ifndef QSYNCDATAINTERFACE_P_H
#define QSYNCDATAINTERFACE_P_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2020 Greedysky Studio

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

#include "qsyncdatainterface.h"

class QSyncDataInterfacePrivate : public TTKPrivate<QSyncDataInterface>
{
public:
    QSyncDataInterfacePrivate()
    {
        m_manager = nullptr;
    }

    void insertAuthorization(const QString &method, TTKStringMap &headers, const QString &resource) const
    {
        if(!QSyncConf::NAME.isEmpty() && !QSyncConf::KEY.isEmpty())
        {
            headers.insert("Authorization", QSyncUtils::createSignForNormalAuth(method, QSyncConf::NAME, QSyncConf::KEY, headers, resource));
        }
        else if(!QSyncConf::NAME.isEmpty())
        {
            headers.insert("Authorization", QSyncConf::NAME);
        }
    }

    QNetworkAccessManager *m_manager;
};

#endif
