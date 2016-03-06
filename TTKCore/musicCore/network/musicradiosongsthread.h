#ifndef MUSICRADIOSONGSTHREAD_H
#define MUSICRADIOSONGSTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicradiothreadabstract.h"

typedef struct SongInfo
{
    QString m_songRealLink;
    QString m_songName;
    QString m_artistName;
    QString m_songPicSmall;
    QString m_songPicRadio;
    QString m_albumName;
    QString m_lrcLink;
    QString m_size;
}SongInfo;
typedef QList<SongInfo> SongInfos;

/*! @brief The class of music radio thread of song info.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicRadioSongsThread : public MusicRadioThreadAbstract
{
    Q_OBJECT

public:
    explicit MusicRadioSongsThread(QObject *parent = 0, QNetworkCookieJar *cookie = NULL);
    /*!
     * Object contsructor.
     */
    virtual ~MusicRadioSongsThread();

    virtual void startToDownload(const QString &id) override;
    /*!
     * Start to download data.
     */
    SongInfo getMusicSongInfo();
    /*!
     * Get music song information.
     */

public Q_SLOTS:
    virtual void downLoadFinished() override;
    /*!
     * Download data from net finished.
     */

protected:
    SongInfo m_songInfo;

};

#endif // MUSICRADIOSONGSTHREAD_H
