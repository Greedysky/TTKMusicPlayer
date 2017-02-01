#ifndef MUSICDOWNLOADQUERYXMARTISTTHREAD_H
#define MUSICDOWNLOADQUERYXMARTISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

const QString XM_SONG_ARTIST_URL    = "alRtVWZ5elpjY3cwUzd1Z25NR04rb2krVDIrUkdsUjU1Y3IwM0UxYUFGT21GVVY3K2EwQkQ1cEhucWRFM00wRQ==";
const QString WY_SONG_QUERY_URL     = "RFJKcUxXdWt5dUhEQ056Rm9RaDhqZW5GSVFBZXZWcXU0bU5QdUZoNlBZY3V3RlFxN25QK1BRPT0=";
const QString WY_SONG_PLAYLIST_URL  = "YWc0dUhjNGdsTFY2VSsyMVBWWnYxMllDYlZrNFNmMEFBb1B5NlRDcGlwNU9tSDdYR010ODB6OHhocGhFQkx1KzJ4Z1R0eHRwVklzTCtkR0M=";

/*! @brief The class to xiami query artist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryXMArtistThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryXMArtistThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &artist) override;
    /*!
     * Start to Search data from name and type.
     */
    void startSearchSong(const QString &artist);
    /*!
     * Start to Search data from name and type.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void songListFinished();
    /*!
     * Query song list finished.
     */

protected:
    void startSongListQuery();
    /*!
     * Start to search song list by id.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */
    int map2NormalBitrate(int bitrate);
    /*!
     * Map json bitrate to normal bitrate.
     */

    QSet<QString> m_songIds;
    int m_index;

};

#endif // MUSICDOWNLOADQUERYXMARTISTTHREAD_H
