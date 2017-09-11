#ifndef MUSICKWDISCOVERLISTTHREAD_H
#define MUSICKWDISCOVERLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloaddiscoverlistthread.h"

/*! @brief The class to kuwo discover toplist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicKWDiscoverListThread : public MusicDownLoadDiscoverListThread
{
    Q_OBJECT
public:
    explicit MusicKWDiscoverListThread(QObject *parent = 0);
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

#endif // MUSICKWDISCOVERLISTTHREAD_H
