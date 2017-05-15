#ifndef MUSICDOWNLOADQUERYBDPLAYLISTTHREAD_H
#define MUSICDOWNLOADQUERYBDPLAYLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadbdinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to baidu query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryBDPlaylistThread : public MusicDownLoadQueryThreadAbstract,
                                                                private MusicDownLoadBDInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryBDPlaylistThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startSearchSong(QueryType type, const QString &playlist) override;
    /*!
     * Start to Search data from name and type.
     */
    virtual void startSearchSong(int offset) override;
    /*!
     * Start to search data from name and type bt paging.
     */
    void startSearchSongAll(const QSet<QString> &ids);
    /*!
     * Start to search all data.
     */
    void startSearchSong(const QString &playlist);
    /*!
     * Start to Search data.
     */

Q_SIGNALS:
    void createPlaylistItems(const MusicPlaylistItem &item);
    /*!
     * Create the current playlist item.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void getSongAllFinished();
    /*!
     * Download song all finished.
     */
    void getDetailsFinished();
    /*!
     * Download details data from net finished.
     */

};

#endif // MUSICDOWNLOADQUERYBDPLAYLISTTHREAD_H
