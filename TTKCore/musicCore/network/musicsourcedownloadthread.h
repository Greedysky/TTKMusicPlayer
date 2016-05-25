#ifndef MUSICSOURCEDOWNLOADTHREAD_H
#define MUSICSOURCEDOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicnetworkabstract.h"

/*! @brief The class of source data download thread.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicSourceDownloadThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    explicit MusicSourceDownloadThread(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicSourceDownloadThread();

    void startToDownload(const QString &url);
    /*!
     * Start to download data.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

};

#endif // MUSICSOURCEDOWNLOADTHREAD_H
