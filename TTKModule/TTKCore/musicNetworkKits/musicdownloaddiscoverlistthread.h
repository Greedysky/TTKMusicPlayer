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
    /*!
     * Object contsructor.
     */
    explicit MusicDownLoadDiscoverListThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to Search data from toplist.
     * Subclass should implement this function.
     */
    virtual void startToSearch() = 0;

    /*!
     * Get top list info.
     */
    inline QString getTopListInfo() const { return m_topListInfo; }

protected:
    QString m_topListInfo;

};

#endif // MUSICDOWNLOADDISCOVERLISTTHREAD_H
