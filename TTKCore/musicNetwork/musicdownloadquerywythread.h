#ifndef MUSICDOWNLOADQUERYWYTHREAD_H
#define MUSICDOWNLOADQUERYWYTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

#define MY_SEARCH_URL         QString("http://music.163.com/api/search/get")
#define MY_SEARCH_QUERY_URL   QString("s=%1&type=1&limit=40&offset=%2")
#define MY_SONG_URL           QString("http://music.163.com/api/song/detail?ids=[%1]")
#define MY_SONG_MV_URL        QString("http://music.163.com/api/mv/detail?id=%1&type=mp4")
#define MY_SONG_PATH_URL      QString("http://m1.music.126.net/%1/%2.mp3")
#define MY_SONG_PIC_URL       QString("http://p4.music.126.net/%1/%2.jpg")
#define MY_SONG_LRC_URL       QString("http://music.163.com/api/song/lyric?lv=-1&id=%1")

#define MY_BASE_URL           "http://music.163.com"
#define MY_ENCRYPT_STRING     "3go8&$8*3*3h0k(2)2"

/*! @brief The class to wangyi query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryWYThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryWYThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startSearchSong(QueryType type, const QString &text) override;
    /*!
     * Start to search data from name and type.
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
    void mvListFinished();
    /*!
     * Query mv list finished.
     */

protected:
    void startSongListQuery();
    /*!
     * Start to search song list by id.
     */
    void startMVListQuery();
    /*!
     * Start to search mv list by id.
     */
    void readFromMusicSongAttribute(MusicObject::MusicSongInfomation *info,
                                    const QVariantMap &key, int bitrate);
    /*!
     * Read tags(size\bitrate\url) from query results.
     */

    QString encryptedId(qlonglong id);
    /*!
     * Encrypted music info id.
     */
    QString encryptedId(const QString &string);
    /*!
     * Encrypted music info id.
     */
    bool foundMusicInfoById(int id);
    /*!
     * Found music info id.
     */

    QSet<QString> m_songIds;
    int m_index;

};

#endif // MUSICDOWNLOADQUERYWYTHREAD_H
