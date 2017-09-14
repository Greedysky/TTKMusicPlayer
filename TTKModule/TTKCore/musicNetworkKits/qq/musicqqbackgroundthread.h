#ifndef MUSICQQBACKGROUNDTHREAD_H
#define MUSICQQBACKGROUNDTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadbackgroundthread.h"

/*! @brief The class of qq download art background image.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicQQBackgroundThread : public MusicDownloadBackgroundThread
{
    Q_OBJECT
public:
    /*!
     * Object contsructor provide artist name and save local path.
     */
    MusicQQBackgroundThread(const QString &name, const QString &save,
                            QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Release the network object.
     */
    virtual void deleteAll() override;
    /*!
     * Start to download artist picture from net.
     */
    virtual void startToDownload() override;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadDataFinished();
    /*!
     * Download data from net finished.
     */
    void downLoadUrlFinished();

protected:
    /*!
     * Download data from net.
     */
    void downLoadUrl(const QString &id);

};

#endif // MUSICQQBACKGROUNDTHREAD_H
