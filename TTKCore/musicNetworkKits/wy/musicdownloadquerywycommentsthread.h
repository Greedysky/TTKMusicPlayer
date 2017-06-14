#ifndef MUSICDOWNLOADQUERYWYCOMMENTSTHREAD_H
#define MUSICDOWNLOADQUERYWYCOMMENTSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadquerythreadabstract.h"

/*! @brief The class to wangyi query song comments download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadQueryWYCommentsThread : public MusicDownLoadQueryThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicDownLoadQueryWYCommentsThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    virtual ~MusicDownLoadQueryWYCommentsThread();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startToSearch(QueryType type, const QString &name) override;
    /*!
     * Start to Search data from name.
     */
    virtual void startToPage(int offset) override;
    /*!
     * Start to search data from name and type bt paging.
     */

Q_SIGNALS:
    void createSearchedItems(const MusicSongCommentItem &comments);
    /*!
     * Create the current song comment.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

};

#endif // MUSICDOWNLOADQUERYWYCOMMENTSTHREAD_H
