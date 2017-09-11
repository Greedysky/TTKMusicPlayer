#ifndef MUSICDOWNLOADDISCOVERLISTTHREAD_H
#define MUSICDOWNLOADDISCOVERLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadpagingthread.h"

/*! @brief The class to download discover toplist abstract thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownLoadDiscoverListThread : public MusicDownLoadPagingThread
{
    Q_OBJECT
public:
    explicit MusicDownLoadDiscoverListThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToSearch() = 0;
    /*!
     * Start to Search data from toplist.
     * Subclass should implement this function.
     */

    inline QString getTopListInfo() const { return m_topListInfo; }
    /*!
     * Get top list info.
     */

protected:
    QString m_topListInfo;

};

#endif // MUSICDOWNLOADDISCOVERLISTTHREAD_H
