#ifndef MUSICQQTEXTDOWNLOADTHREAD_H
#define MUSICQQTEXTDOWNLOADTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadthreadabstract.h"

/*! @brief The class of downloading the type of qq txt.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicQQTextDownLoadThread : public MusicDownLoadThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor provide download URL\ save local path and download type.
     */
    MusicQQTextDownLoadThread(const QString &url, const QString &save,
                              Download_Type type, QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     */
    virtual void startToDownload() override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

};

#endif // MUSICQQTEXTDOWNLOADTHREAD_H
