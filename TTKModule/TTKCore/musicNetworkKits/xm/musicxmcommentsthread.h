#ifndef MUSICXMCOMMENTSTHREAD_H
#define MUSICXMCOMMENTSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadxminterface.h"
#include "musicdownloadcommentsthread.h"

/*! @brief The class to xiami query song comments download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicXMCommentsThread : public MusicDownLoadCommentsThread,
                                                   private MusicDownLoadXMInterface
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicXMCommentsThread(QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();

    /*!
     * Start to Search data from name.
     */
    virtual void startToSearch(const QString &name) override;
    /*!
     * Start to search data from name and type bt paging.
     */
    virtual void startToPage(int offset) override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

};

#endif // MUSICXMCOMMENTSTHREAD_H
