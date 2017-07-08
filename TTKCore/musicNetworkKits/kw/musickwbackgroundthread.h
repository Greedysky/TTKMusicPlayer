#ifndef MUSICKWBACKGROUNDTHREAD_H
#define MUSICKWBACKGROUNDTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicdownloadbackgroundthread.h"

/*! @brief The class of kuwo download art background image.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicKWBackgroundThread : public MusicDownloadBackgroundThread
{
    Q_OBJECT
public:
    MusicKWBackgroundThread(const QString &name, const QString &save,
                            QObject *parent = 0);
    /*!
     * Object contsructor provide artist name and save local path.
     */

    static QString getClassName();
    /*!
     * Get class object name.
     */
    virtual void startToDownload() override;
    /*!
     * Start to download artist picture from net.
     */

public Q_SLOTS:
    void downLoadDataFinished(const QByteArray &bytes);
    /*!
     * Download data from kuwo net finished.
     */

};

#endif // MUSICKWBACKGROUNDTHREAD_H
