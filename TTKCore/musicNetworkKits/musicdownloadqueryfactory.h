#ifndef MUSICDOWNLOADQUERYFACTORY_H
#define MUSICDOWNLOADQUERYFACTORY_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsingleton.h"
#include "musicdownloadthreadabstract.h"

#define M_DOWNLOAD_QUERY_PTR (MusicSingleton<MusicDownLoadQueryFactory>::createInstance())

class MusicDownLoadQueryThreadAbstract;

/*! @brief The class to produce the downlaod query class by type.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryFactory
{
public:
    static QString getClassName();
    /*!
     * Get class object name.
     */

    MusicDownLoadQueryThreadAbstract *getQueryThread(QObject *parent);
    /*!
     * Get query thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getAlbumThread(QObject *parent);
    /*!
     * Get album thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getArtistThread(QObject *parent);
    /*!
     * Get artist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getPlaylistThread(QObject *parent);
    /*!
     * Get playlist thread object by type.
     */
    MusicDownLoadThreadAbstract *getDownloadSmallPic(const QString &url, const QString &save,
                                                     MusicDownLoadThreadAbstract::Download_Type type,
                                                     QObject *parent = 0);
    /*!
     * Get download small picture object by type.
     */
    MusicDownLoadThreadAbstract *getDownloadLrc(const QString &url, const QString &save,
                                                MusicDownLoadThreadAbstract::Download_Type type,
                                                QObject *parent = 0);
    /*!
     * Get download lrc object by type.
     */

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#endif // MUSICDOWNLOADQUERYFACTORY_H
