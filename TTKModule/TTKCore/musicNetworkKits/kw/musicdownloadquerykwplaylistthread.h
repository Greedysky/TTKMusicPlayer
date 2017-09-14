#ifndef MUSICDOWNLOADQUERYKWPLAYLISTTHREAD_H
#define MUSICDOWNLOADQUERYKWPLAYLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadkwinterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to kuwo query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryKWPlaylistThread : public MusicDownLoadQueryThreadAbstract,
                                                                private MusicDownLoadKWInterface
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadQueryKWPlaylistThread(QObject *parent = 0);

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
     * Download details data from net finished.
     */
    void getDetailsFinished();
    /*!
     * Get more playlisy details finished.
     */
    void getMorePlaylistDetailsFinished();

protected:
    /*!
     * Get more playlisy details.
     */
    void getMorePlaylistDetails(const QString &pid);

    QString m_tags;

};

#endif // MUSICDOWNLOADQUERYKWPLAYLISTTHREAD_H
