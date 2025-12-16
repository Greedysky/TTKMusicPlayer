#ifndef MUSICCONNECTIONPOOL_H
#define MUSICCONNECTIONPOOL_H

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

#include <QMap>
#include "ttksingleton.h"

#define TTK_CONNECTIONPOOL_REGISTER(object) G_CONNECTION_PTR->setValue(className(), object)
#define TTK_CONNECTIONPOOL_UNREGISTER(object) G_CONNECTION_PTR->removeValue(object)
#define TTK_CONNECTIONPOOL_BIND_TO(object) G_CONNECTION_PTR->connect(className(), object::className())
#define TTK_CONNECTIONPOOL_BIND_FROM(object) G_CONNECTION_PTR->connect(object::className(), className())

/*! @brief The class of the qt signal and slot connection pool.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicConnectionPool
{
    TTK_DECLARE_SINGLETON_CLASS(MusicConnectionPool)
public:
    /*!
     * Set connection object and type name.
     */
    inline void setValue(const QString &type, QObject *object) noexcept
    {
        m_parameters[type] = object;
    }

    /*!
     * Get connection object by type name.
     */
    inline QObject* value(const QString &type) const noexcept
    {
        return m_parameters[type];
    }

    /*!
     * Remove connection object by object.
     */
    void removeValue(const QObject *object);

    /*!
     * Set connection by two object type name.
     */
    void connect(const QString &from, const QString &to);

private:
    /*!
     * Object constructor.
     */
    MusicConnectionPool() = default;
    
    QMap<QString, QObject*> m_parameters;

};

#define G_CONNECTION_PTR makeMusicConnectionPool()
TTK_MODULE_EXPORT MusicConnectionPool* makeMusicConnectionPool();

#endif // MUSICCONNECTIONPOOL_H
