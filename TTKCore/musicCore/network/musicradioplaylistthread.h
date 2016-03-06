#ifndef MUSICRADIOPLAYLISTTHREAD_H
#define MUSICRADIOPLAYLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicradiothreadabstract.h"

/*! @brief The class of music radio thread of playlist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioPlayListThread : public MusicRadioThreadAbstract
{
    Q_OBJECT
public:
    explicit MusicRadioPlayListThread(QObject *parent = 0, QNetworkCookieJar *cookie = NULL);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRadioPlayListThread();

    virtual void startToDownload(const QString &id) override;
    /*!
     * Start to download data.
     */
    QStringList getMusicPlayList();
    /*!
     * Get music playlist.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    QStringList m_playList;

};

#endif // MUSICRADIOPLAYLISTTHREAD_H
