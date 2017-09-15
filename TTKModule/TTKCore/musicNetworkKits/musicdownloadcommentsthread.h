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

/*! @brief The class of the song comment item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_NETWORK_EXPORT MusicSongCommentItem
{
    QString m_nickName;
    QString m_avatarUrl;
    QString m_time;
    QString m_content;
    QString m_likedCount;
}MusicSongCommentItem;
MUSIC_DECLARE_LISTS(MusicSongCommentItem)

/*! @brief The class to query song comments download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadCommentsThread : public MusicDownLoadPagingThread
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadCommentsThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to Search data from name.
     * Subclass should implement this function.
     */
    virtual void startToSearch(const QString &name) = 0;

Q_SIGNALS:
    /*!
     * Create the current song comment.
     */
    void createSearchedItems(const MusicSongCommentItem &comments);

};

#endif // MUSICDOWNLOADCOMMENTSTHREAD_H
