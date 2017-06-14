#ifndef MUSICDOWNLOADCOMMENTSTHREAD_H
#define MUSICDOWNLOADCOMMENTSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicdownloadpagingthread.h"

typedef struct MUSIC_NETWORK_EXPORT MusicSongCommentItem
{
    QString m_nickName;
    QString m_avatarUrl;
    QString m_time;
    QString m_content;
    QString m_likedCount;
}MusicSongCommentItem;
TTK_DECLARE_LISTS(MusicSongCommentItem)

/*! @brief The class to query song comments download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadCommentsThread : public MusicDownLoadPagingThread
{
    Q_OBJECT
public:
    explicit MusicDownLoadCommentsThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void startToSearch(const QString &name) = 0;
    /*!
     * Start to Search data from name.
     * Subclass should implement this function.
     */

Q_SIGNALS:
    void createSearchedItems(const MusicSongCommentItem &comments);
    /*!
     * Create the current song comment.
     */

};

#endif // MUSICDOWNLOADCOMMENTSTHREAD_H
