#ifndef MUSICDOWNLOADBACKGROUNDREMOTETHREAD_H
#define MUSICDOWNLOADBACKGROUNDREMOTETHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicabstractxml.h"

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
        m_name = "Greedysky";
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


/*! @brief The class of the remote skin XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicSkinRemoteConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    explicit MusicSkinRemoteConfigManager(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void readSkinRemoteXMLConfig(MusicSkinRemoteGroups &items);
    /*!
     * Read datas into xml file.
     */

};


/*! @brief The class of download skin remote background.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadBackgroundRemoteThread : public QObject
{
    Q_OBJECT
public:
    MusicDownloadBackgroundRemoteThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    void startToDownload();
    /*!
     * Start to download data from net.
     */

Q_SIGNALS:
    void downLoadDataChanged(const MusicSkinRemoteGroups &data);
    /*!
     * Send download data from net.
     */

public Q_SLOTS:
    void downLoadDataFinished(const QByteArray &bytes);
    /*!
     * Download data from net finished.
     */

};

#endif // MUSICDOWNLOADBACKGROUNDREMOTETHREAD_H
