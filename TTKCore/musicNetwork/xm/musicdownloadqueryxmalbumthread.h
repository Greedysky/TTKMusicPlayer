#ifndef MUSICDOWNLOADQUERYXMALBUMTHREAD_H
#define MUSICDOWNLOADQUERYXMALBUMTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString XM_SONG_ALBUM_URL    = "SlEzVGhEWWhhMDJBaVJFUWJoejFJZHFQMUhDTktmWExESzh4dHY5ZlhhSStmU2dFV3FabzdHcG1tZk9Sa013aUtVb2dTQ1FuQWtGL1ZndGo=";

/*! @brief The class to xiami query album download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryXMAlbumThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryXMAlbumThread(QObject *parent = 0);
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
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

};

#endif // MUSICDOWNLOADQUERYXMALBUMTHREAD_H
