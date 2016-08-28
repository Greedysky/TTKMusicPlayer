#ifndef MUSICDOWNLOADQUERYALBUMTTTHREAD_H
#define MUSICDOWNLOADQUERYALBUMTTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString TT_SONG_SINGER_URL   = "http://api.dongting.com/song/singer/%1/songs?page=1&size=50";
const QString TT_SONG_ALBUM_URL    = "http://api.dongting.com/song/album/%1/songs?page=1&size=50";

/*! @brief The class to ttpod query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryAlbumTTThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryAlbumTTThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &text) override;
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


};

#endif // MUSICDOWNLOADQUERYALBUMTTTHREAD_H
