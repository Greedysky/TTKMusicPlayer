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
MUSIC_DECLARE_LISTS(MusicSkinRemoteItem)

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
MUSIC_DECLARE_LISTS(MusicSkinRemoteGroup)


/*! @brief The class of the remote skin XML Config Manager.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicSkinRemoteConfigManager : public MusicAbstractXml
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSkinRemoteConfigManager(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Read datas into xml file.
     */
    void readSkinRemoteXMLConfig(MusicSkinRemoteGroups &items);

};


/*! @brief The class of download skin remote background.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadBackgroundRemoteThread : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadBackgroundRemoteThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to download data from net.
     */
    void startToDownload();

Q_SIGNALS:
    /*!
     * Send download data from net.
     */
    void downLoadDataChanged(const MusicSkinRemoteGroups &data);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadDataFinished(const QByteArray &bytes);

};

#endif // MUSICDOWNLOADBACKGROUNDREMOTETHREAD_H
