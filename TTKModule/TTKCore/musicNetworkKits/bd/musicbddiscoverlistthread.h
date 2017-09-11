#ifndef MUSICBDDISCOVERLISTTHREAD_H
#define MUSICBDDISCOVERLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloaddiscoverlistthread.h"

/*! @brief The class to baidu discover toplist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicBDDiscoverListThread : public MusicDownLoadDiscoverListThread
{
    Q_OBJECT
public:
    explicit MusicBDDiscoverListThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToSearch() override;
    /*!
     * Start to Search data from toplist.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */
    void searchTopListInfoFinished();
    /*!
     * Search top list information finished.
     */

protected:
    void searchTopListInformation(const QString &id);
    /*!
     * Search top list information.
     */

};

#endif // MUSICBDDISCOVERLISTTHREAD_H
