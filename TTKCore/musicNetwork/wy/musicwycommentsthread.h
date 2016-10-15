#ifndef MUSICWYCOMMENTSTHREAD_H
#define MUSICWYCOMMENTSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicnetworkabstract.h"

typedef struct MUSIC_NETWORK_EXPORT MusicSongComment
{
    QString m_nickName;
    QString m_avatarUrl;
    QString m_time;
    QString m_content;
    QString m_likedCount;
}MusicSongInfomation;

#define COMMIT_PAGE_SIZE    10
const QString WY_SONG_COMMIT_URL = "TFFwV2I3RndydWRyYUFScm9RZjFCVlg0OW5OQy9LSlVFQTJWRUxraDhHNFNrVDFsWk5vaEtDeHNyTXNQOTRYNnVtQmVBTVVLUytBalJTZ3Y0ZTI3T2ZkSjJFdXVNbzZCWnNRZnRBbXBjSitGQUVpTkticGVLYWU1RWZ3aTAxbTFDdnZFYm9Kb0NSY2JiYmpU";

/*! @brief The class to wangyi query song comments download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicWYCommentsThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    explicit MusicWYCommentsThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    virtual ~MusicWYCommentsThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void deleteAll();
    /*!
     * Release the network object.
     */
    void startSearchSong(const QString &name);
    /*!
     * Start to Search data from name.
     */
    void startSearchSong(int index);
    /*!
     * Start to Search data from page index.
     */
    inline int total() const { return m_count;}
    /*!
     * Get the current comments all count.
     */

Q_SIGNALS:
    void createSearchedItems(const MusicSongComment &comments);
    /*!
     * Create the current song comment.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    int m_count, m_songID;

};

#endif // MUSICWYCOMMENTSTHREAD_H
