#ifndef MUSICDOWNLOADMANAGER_H
#define MUSICDOWNLOADMANAGER_H

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

#include "ttksingleton.h"
#include "musicnetworkdefines.h"

/*! @brief The class of the download manger data.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicDownLoadData
{
    qint64 m_timestamp;
    QObject *m_object;
    TTK::Record m_type;

    MusicDownLoadData() noexcept
        : m_timestamp(-1),
          m_object(nullptr),
          m_type(TTK::Record::NormalDownload)
    {

    }

    MusicDownLoadData(qint64 t) noexcept
        : MusicDownLoadData()
    {
        m_timestamp = t;
    }

    MusicDownLoadData(qint64 t, QObject *object, TTK::Record type) noexcept
        : m_timestamp(t),
          m_object(object),
          m_type(type)
    {

    }

    inline bool operator< (const MusicDownLoadData &other) const noexcept
    {
        return m_timestamp < other.m_timestamp;
    }

    inline bool operator== (const MusicDownLoadData &other) const noexcept
    {
        return m_timestamp == other.m_timestamp;
    }
};

/*! @brief The class of the produce the download manager.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownLoadManager : public QObject
{
    Q_OBJECT
    TTK_DECLARE_SINGLETON_CLASS(MusicDownLoadManager)
public:
    /*!
     * Set mutiple network connection object.
     */
    void connectMultiNetwork(QObject *object);
    /*!
     * Remove mutiple network connection object.
     */
    void removeMultiNetwork(QObject *object);

    /*!
     * Set data network connection object.
     */
    void connectNetworkData(const MusicDownLoadData &data);
    /*!
     * Reset data network connection object.
     */
    void reconnectNetworkData(const MusicDownLoadData &data);
    /*!
     * Remove data network connection object.
     */
    void removeNetworkData(const MusicDownLoadData &data);

private Q_SLOTS:
    /*!
     * Update download percent total time and current time progress.
     */
    void downloadProgressChanged(float percent, const QString &total, qint64 time);

private:
    QObjectList m_objects;
    QList<MusicDownLoadData> m_datas;

};

#define G_DOWNLOAD_MANAGER_PTR makeMusicDownLoadManager()
TTK_MODULE_EXPORT MusicDownLoadManager* makeMusicDownLoadManager();

#endif // MUSICDOWNLOADMANAGER_H
