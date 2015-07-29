#ifndef QMUSICPLAYER_H
#define QMUSICPLAYER_H

#include <QTimer>
#include "musicequalizer.h"

#ifdef Q_OS_WIN32
    #include <Windows.h>
#endif

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

    MusicPlayer(QObject *parent = 0);
    ~MusicPlayer();

    State state() const;
    MusicPlaylist *playlist() const;

    qint64 duration() const;
    qint64 position() const;

    int volume() const;
    bool isMuted() const;

    void setPlay3DMusicFlag(bool&);

signals:
    void stateChanged();
    void stateChanged(MusicPlayer::State newState);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

    void volumeChanged(int volume);
    void mutedChanged(bool muted);

public slots:
    void play();
    void pause();
    void stop();
    void playNext();
    void playPrivious();

    void setPosition(qint64 position);
    void setVolume(int volume);
    void setMuted(bool muted);

    void setPlaylist(MusicPlaylist *playlist);
#ifdef Q_OS_WIN32
    void setSpectrum(HWND,int w,int h,int x = 0,int y = 0);
#endif

    void setTimeOut();
    void removeCurrentMedia();

    void setEqEffect(const MIntList& hz);
    void setEnaleEffect(bool b);
    void setSpEqEffect(MusicObject::SpecialEQ);
    void setEqInformation();
    void resetEqEffect();

protected:
    MusicEqualizer* m_equalizer;
    MusicPlaylist* m_playlist;
    State m_state;
    ZPlay* m_music;
    QTimer m_timer;
    QString m_currentMedia;

    int m_currentVolumn;
    bool m_play3DMusic;
    float m_posOnCircle;

};

#endif // QMUSICPLAYER_H
