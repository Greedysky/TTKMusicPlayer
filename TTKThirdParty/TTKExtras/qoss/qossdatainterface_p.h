#ifndef QOSSDATAINTERFACE_P_H
#define QOSSDATAINTERFACE_P_H

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

#include "qossdatainterface.h"

class QOSSDataInterfacePrivate : public TTKPrivate<QOSSDataInterface>
{
public:
    QOSSDataInterfacePrivate()
    {
        m_manager = nullptr;
    }

    void insertAuthorization(const QString &method, TTKStringMap &headers, const QString &resource) const
    {
        if(!QOSSConf::ACCESS_KEY.isEmpty() && !QOSSConf::SECRET_KEY.isEmpty())
        {
            headers.insert("Authorization", QOSSUtils::createSignForNormalAuth(method, QOSSConf::ACCESS_KEY, QOSSConf::SECRET_KEY, headers, resource));
        }
        else if(!QOSSConf::ACCESS_KEY.isEmpty())
        {
            headers.insert("Authorization", QOSSConf::ACCESS_KEY);
        }
    }

    QNetworkAccessManager *m_manager;
};

#endif // QOSSDATAINTERFACE_P_H
