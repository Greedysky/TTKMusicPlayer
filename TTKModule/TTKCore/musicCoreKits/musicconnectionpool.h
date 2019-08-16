#ifndef MUSICCONNECTIONPOOL_H
#define MUSICCONNECTIONPOOL_H

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

#include <QMap>
#include "musicsingleton.h"

#define M_CONNECTION_PTR (MusicSingleton<MusicConnectionPool>::createInstance())

/*! @brief The class of the qt signal and slot connection pool.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicConnectionPool
{
    TTK_DECLARE_MODULE(MusicConnectionPool)
public:
    /*!
     * Set connection object and type name.
     */
    inline void setValue(const QString &type, QObject *object)
    {
        m_parameter[type] = object;
    }
    /*!
     * Get connection object by type name.
     */
    inline QObject* value(const QString &type) const
    {
        return m_parameter[type];
    }
    /*!
     * Get connection object by type name in operator[].
     */
    inline const QObject* operator[](const QString &type) const
    {
        return value(type);
    }
    /*!
     * Set connection object and type name in operator[].
     */
    inline QObject* &operator[](const QString &type)
    {
        return m_parameter[type];
    }

    /*!
     * Set connection by two object.
     */
    void poolConnect(const QObject *from, const QObject *to);
    /*!
     * Set connection by two object type name.
     */
    void poolConnect(const QString &from, const QString &to);
    /*!
     * Remove connection object by object.
     */
    void removeValue(const QObject *object);
    /*!
     * Remove connection object by object name.
     */
    void removeValue(const QString &name);

protected:
    QMap<QString, QObject*> m_parameter;

    /*!
     * Object contsructor.
     */
    MusicConnectionPool();

    DECLARE_SINGLETON_CLASS(MusicConnectionPool)

};

#endif // MUSICCONNECTIONPOOL_H
