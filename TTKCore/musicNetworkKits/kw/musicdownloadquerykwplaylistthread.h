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
    explicit MusicDownLoadQueryKWPlaylistThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startToSearch(QueryType type, const QString &playlist) override;
    /*!
     * Start to Search data from name and type.
     */
    virtual void startToPage(int offset) override;
    /*!
     * Start to search data from name and type bt paging.
     */
    void startToSearch(const QString &playlist);
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
    void getMorePlaylistDetailsFinished();
    /*!
     * Get more playlisy details finished.
     */

protected:
    void getMorePlaylistDetails(const QString &pid);
    /*!
     * Get more playlisy details.
     */

    QString m_tags;

};

#endif // MUSICDOWNLOADQUERYKWPLAYLISTTHREAD_H
