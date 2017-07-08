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
    MusicQQBackgroundThread(const QString &name, const QString &save,
                            QObject *parent = 0);
    /*!
     * Object contsructor provide artist name and save local path.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void deleteAll() override;
    /*!
     * Release the network object.
     */
    virtual void startToDownload() override;
    /*!
     * Start to download artist picture from net.
     */

public Q_SLOTS:
    void downLoadDataFinished();
    /*!
     * Download data from net finished.
     */
    void downLoadUrlFinished();
    /*!
     * Download data from net finished.
     */

protected:
    void downLoadUrl(const QString &id);
    /*!
     * Download data from net.
     */

};

#endif // MUSICQQBACKGROUNDTHREAD_H
