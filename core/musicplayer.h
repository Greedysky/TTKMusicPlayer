#ifndef QMUSICPLAYER_H
#define QMUSICPLAYER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "musicequalizer.h"

#ifdef Q_OS_WIN32
    #include <Windows.h>
#endif

class QMediaPlayer;
class MusicPlaylist;

class MUSIC_CORE_EXPORT MusicPlayer : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    enum Enhanced
    {
        EnhancedOff,
        Music3D,
        MusicVocal,
        MusicNICAM,
        MusicSubwoofer
    };

    MusicPlayer(QObject *parent = 0);
    ~MusicPlayer();

    State state() const;
    void setPlaylist(MusicPlaylist *playlist);
    MusicPlaylist *playlist() const;

    qint64 duration() const;
    qint64 position() const;

    void playNext();
    void playPrivious();

    int volume() const;
    void setVolume(int volume);
    bool isMuted() const;
    void setMuted(bool muted);
    void setPosition(qint64 position);

    void setMusicEnhanced(Enhanced type);
    Enhanced getMusicEnhanced() const;

    void addAuditionUrl(const QString &url);
    void addAuditionUrl(const QUrl &url);
    void startAudition();
    void stopAudition();

    static QStringList supportFormatsString();
    static QStringList supportFormatsFilterString();
    static QStringList supportFormatsFilterDialogString();

signals:
    void stateChanged(MusicPlayer::State newState);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

    void volumeChanged(int volume);
    void mutedChanged(bool muted);

public slots:
    void play();
    void pause();
    void stop();

    void setEqEffect(const MIntList &hz);
    void setEnaleEffect(bool enable);
    void setSpEqEffect(MusicObject::SpecialEQ eq);
    void setEqInformation();
    void resetEqEffect();
#ifdef Q_OS_WIN32
    void setSpectrum(HWND,int w,int h,int x = 0,int y = 0);
#endif

private slots:
    void setTimeOut();
    void removeCurrentMedia();

protected:
    void setMusicEnhancedCase();

    QMediaPlayer *m_audition;
    MusicEqualizer *m_equalizer;
    MusicPlaylist *m_playlist;
    State m_state;
    ZPlay *m_music;
    QTimer m_timer;
    QString m_currentMedia;
    Enhanced m_musicEnhanced;

    int m_currentVolumn;
    float m_posOnCircle;

};

#endif // QMUSICPLAYER_H
