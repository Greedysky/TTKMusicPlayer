#ifndef MUSICDOWNLOADBACKGROUNDTHREAD_H
#define MUSICDOWNLOADBACKGROUNDTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicobject.h"
#include "musicnetworkabstract.h"

/*! @brief The class of download art background image.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadBackgroundThread : public MusicNetworkAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor provide artist name and save local path.
     */
    MusicDownloadBackgroundThread(const QString &name, const QString &save,
                                  QObject *parent = 0);

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download artist picture from net.
     * Subclass should implement this function.
     */
    virtual void startToDownload() = 0;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    QString m_artName, m_savePath;
    int m_index, m_counter;

};

#endif // MUSICDOWNLOADBACKGROUNDTHREAD_H
