#ifndef MUSICDOWNLOADQUERYKGPLAYLISTTHREAD_H
#define MUSICDOWNLOADQUERYKGPLAYLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadkginterface.h"
#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to kugou query playlist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryKGPlaylistThread : public MusicDownLoadQueryThreadAbstract,
                                                                private MusicDownLoadKGInterface
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryKGPlaylistThread(QObject *parent = 0);
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

};

#endif // MUSICDOWNLOADQUERYKGPLAYLISTTHREAD_H
