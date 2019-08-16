#ifndef MUSICDOWNLOADBACKGROUNDREMOTETHREAD_H
#define MUSICDOWNLOADBACKGROUNDREMOTETHREAD_H

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

#include "musicabstractxml.h"
#include "musicotherdefine.h"

/*! @brief The class of the remote skin item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicSkinRemoteItem
{
    QString m_name;
    QString m_url;
    int m_useCount;

    MusicSkinRemoteItem()
    {
        m_name = MUSIC_AUTHOR_NAME;
        m_useCount = 8888;
    }

    bool isValid() const
    {
        return !(m_name.isEmpty() && m_useCount == 0);
    }

}MusicSkinRemoteItem;
TTK_DECLARE_LISTS(MusicSkinRemoteItem)

/*! @brief The class of the remote skin item group.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicSkinRemoteGroup
{
    QString m_group;
    MusicSkinRemoteItems m_items;

    bool isValid() const
    {
        return !(m_group.isEmpty() && m_items.isEmpty());
    }

}MusicSkinRemoteGroup;
TTK_DECLARE_LISTS(MusicSkinRemoteGroup)


/*! @brief The class of the thunder skin XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicSkinThunderConfigManager : public MusicAbstractXml
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSkinThunderConfigManager)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSkinThunderConfigManager(QObject *parent = nullptr);

    /*!
     * Read datas from config file.
     */
    void readSkinRemoteData(MusicSkinRemoteGroups &items);

};


/*! @brief The class of download skin remote background.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadBackgroundRemoteThread : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadBackgroundRemoteThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadBackgroundRemoteThread(QObject *parent = nullptr);

    /*!
     * Start to download data from net.
     */
    virtual void startToDownload() = 0;

Q_SIGNALS:
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const MusicSkinRemoteGroups &data);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadDataFinished(const QByteArray &bytes) = 0;

};


/*! @brief The class of download thunder skin background.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadBackgroundThunderThread : public MusicDownloadBackgroundRemoteThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadBackgroundThunderThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadBackgroundThunderThread(QObject *parent = nullptr);

    /*!
     * Start to download data from net.
     */
    virtual void startToDownload() override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadDataFinished(const QByteArray &bytes) override;

};


/*! @brief The class of download bing daily skin background.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadBackgroundBingThread : public MusicDownloadBackgroundRemoteThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadBackgroundBingThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadBackgroundBingThread(QObject *parent = nullptr);

    /*!
     * Start to download data from net.
     */
    virtual void startToDownload() override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadDataFinished(const QByteArray &bytes) override;

};


#endif // MUSICDOWNLOADBACKGROUNDREMOTETHREAD_H
