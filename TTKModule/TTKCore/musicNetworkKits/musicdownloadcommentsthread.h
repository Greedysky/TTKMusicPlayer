#ifndef MUSICDOWNLOADCOMMENTSTHREAD_H
#define MUSICDOWNLOADCOMMENTSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

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
