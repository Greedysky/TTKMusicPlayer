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

class MusicDownLoadCommentsThread;
class MusicDownLoadDiscoverListThread;
class MusicDownLoadQueryThreadAbstract;
class MusicDownloadBackgroundThread;

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

    MusicDownLoadQueryThreadAbstract *getQueryThread(QObject *parent = 0);
    /*!
     * Get query thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getAlbumThread(QObject *parent = 0);
    /*!
     * Get album thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getArtistThread(QObject *parent = 0);
    /*!
     * Get artist thread object by type.
     */
    MusicDownLoadQueryThreadAbstract *getPlaylistThread(QObject *parent = 0);
    /*!
     * Get playlist thread object by type.
     */

    MusicDownLoadCommentsThread *getCommentThread(QObject *parent = 0);
    /*!
     * Get comment thread object by type.
     */

    MusicDownLoadDiscoverListThread *getDiscoverListThread(QObject *parent = 0);
    /*!
     * Get discover list thread object by type.
     */

    MusicDownLoadThreadAbstract *getDownloadSmallPicThread(const QString &url, const QString &save,
                                                           MusicDownLoadThreadAbstract::Download_Type type,
                                                           QObject *parent = 0);
    /*!
     * Get download small picture object by type.
     */
    MusicDownLoadThreadAbstract *getDownloadLrcThread(const QString &url, const QString &save,
                                                      MusicDownLoadThreadAbstract::Download_Type type,
                                                      QObject *parent = 0);
    /*!
     * Get download lrc object by type.
     */

    MusicDownloadBackgroundThread *getDownloadBigPicThread(const QString &name, const QString &save,
                                                           QObject *parent = 0);
    /*!
     * Get download big picture object by type.
     */

protected:
    DECLARE_SINGLETON_CLASS(MusicDownLoadQueryFactory)

};

#endif // MUSICDOWNLOADQUERYFACTORY_H
