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
    /*!
     * Object contsructor provide artist name and save local path.
     */
    MusicKWBackgroundThread(const QString &name, const QString &save,
                            QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download artist picture from net.
     */
    virtual void startToDownload() override;

public Q_SLOTS:
    /*!
     * Download data from kuwo net finished.
     */
    void downLoadDataFinished(const QByteArray &bytes);

};

#endif // MUSICKWBACKGROUNDTHREAD_H
