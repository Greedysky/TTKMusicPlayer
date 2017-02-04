#ifndef MUSICDOWNLOADQUERYKGALBUMTHREAD_H
#define MUSICDOWNLOADQUERYKGALBUMTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadkginterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to kugou query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryKGAlbumThread : public MusicDownLoadQueryThreadAbstract,
                                                             private MusicDownLoadKGInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryKGAlbumThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &album) override;
    /*!
     * Start to Search data from name and type.
     */
    void startSearchSong(const QString &album);
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    void readFromMusicSongAlbumInfo(MusicObject::MusicSongInfomation *info);
    /*!
     * Read tags(album info) from query results.
     */

};

#endif // MUSICDOWNLOADQUERYKGALBUMTHREAD_H
