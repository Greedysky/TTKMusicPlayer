#ifndef MUSICDOWNLOADQUERYWYPLAYLISTTHREAD_H
#define MUSICDOWNLOADQUERYWYPLAYLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadwyinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to wangyi query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryWYPlaylistThread : public MusicDownLoadQueryThreadAbstract,
                                                                private MusicDownLoadWYInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryWYPlaylistThread(QObject *parent = 0);
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
    void startSearchSongAll(const QString &type);
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
    void getDetailsFinished();
    /*!
     * Download details data from net finished.
     */

};

#endif // MUSICDOWNLOADQUERYWYPLAYLISTTHREAD_H
