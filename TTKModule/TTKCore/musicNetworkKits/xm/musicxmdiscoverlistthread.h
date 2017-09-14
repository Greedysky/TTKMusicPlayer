#ifndef MUSICXMDISCOVERLISTTHREAD_H
#define MUSICXMDISCOVERLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadxminterface.h"
#include "musicdownloaddiscoverlistthread.h"

/*! @brief The class to xiami discover toplist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicXMDiscoverListThread : public MusicDownLoadDiscoverListThread,
                                                       private MusicDownLoadXMInterface
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicXMDiscoverListThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to Search data from toplist.
     */
    virtual void startToSearch() override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

};

#endif // MUSICXMDISCOVERLISTTHREAD_H
