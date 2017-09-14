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
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryBDPlaylistThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to Search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &playlist) override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset) override;
    /*!
     * Start to search all data.
     */
    void startToSearchAll(const QSet<QString> &ids);
    /*!
     * Start to Search data.
     */
    void startToSearch(const QString &playlist);

Q_SIGNALS:
    /*!
     * Create the current playlist item.
     */
    void createPlaylistItems(const MusicPlaylistItem &item);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;
    /*!
     * Download song all finished.
     */
    void getSongAllFinished();
    /*!
     * Download details data from net finished.
     */
    void getDetailsFinished();

};

#endif // MUSICDOWNLOADQUERYBDPLAYLISTTHREAD_H
