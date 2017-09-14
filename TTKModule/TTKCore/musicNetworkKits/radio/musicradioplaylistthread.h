#ifndef MUSICRADIOPLAYLISTTHREAD_H
#define MUSICRADIOPLAYLISTTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QStringList>
#include "musicradiothreadabstract.h"

/*! @brief The class of music radio thread of playlist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioPlayListThread : public MusicRadioThreadAbstract
{
    Q_OBJECT
public:
    /*!
     * Object contsructor.
     */
    explicit MusicRadioPlayListThread(QObject *parent = 0, QNetworkCookieJar *cookie = 0);

    virtual ~MusicRadioPlayListThread();

    /*!
     * Get class object name.
     */
    static QString getClassName();
    /*!
     * Start to download data.
     */
    virtual void startToDownload(const QString &id) override;
    /*!
     * Get music playlist.
     */
    QStringList getMusicPlayList();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override;

protected:
    QStringList m_playList;

};

#endif // MUSICRADIOPLAYLISTTHREAD_H
