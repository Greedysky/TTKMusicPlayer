#ifndef TTKRADIOHELPER_H
#define TTKRADIOHELPER_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musicmobileglobaldefine.h"

class QMediaPlayer;
class QNetworkCookieJar;
class MusicRadioChannelThread;
class MusicRadioSongsThread;
class MusicRadioPlayListThread;

/*! @brief The class of the radio network helper.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKRadioHelper : public QObject
{
    Q_OBJECT
public:
    explicit TTKRadioHelper(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~TTKRadioHelper();

    Q_INVOKABLE void init();
    /*!
     * Init the radio network.
     */
    Q_INVOKABLE bool isPlaying() const;
    /*!
     * Get current player is running or not.
     */

Q_SIGNALS:
    void picDownloadFinished(const QString &path);
    /*!
     * Artist small pic download finished.
     */

public Q_SLOTS:
    void play();
    /*!
     * Set current player play.
     */
    void pause();
    /*!
     * Set current player pause.
     */

private Q_SLOTS:
    void playStateChanged();
    /*!
     * Current play state changed.
     */
    void getChannelFinished();
    /*!
     * Get channel finished.
     */
    void getPlayListFinished();
    /*!
     * Get playList finished.
     */
    void getSongInfoFinished();
    /*!
     * Get song information finished.
     */
    void picDownloadStateChanged();
    /*!
     * Artist small pic download changed.
     */

protected:
    QNetworkCookieJar *m_cookJar;
    MusicRadioChannelThread *m_getChannelThread;
    MusicRadioPlayListThread *m_playListThread;
    MusicRadioSongsThread *m_songsThread;
    QStringList m_playListIds;
    QMediaPlayer *m_player;

};

#endif // TTKRADIOHELPER_H
