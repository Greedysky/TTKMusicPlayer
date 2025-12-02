#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QTimer>

#ifdef Q_OS_WIN
#  include <Windows.h>
#  ifdef Q_CC_GNU
#    pragma GCC diagnostic ignored "-Wswitch"
#  endif
#endif

class SoundCore;
class MusicPlaylist;

class MusicPlayer : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    explicit MusicPlayer(QObject *parent = nullptr);
    ~MusicPlayer();

    State state() const;
    void setPlaylist(MusicPlaylist *playlist);
    MusicPlaylist *playlist() const;

    qint64 duration() const;
    qint64 position() const;

    int volume() const;
    void setVolume(int volume);
    bool isMuted() const;
    void setMuted(bool muted);
    void setPosition(qint64 position);

Q_SIGNALS:
    void stateChanged(MusicPlayer::State newState);

    void durationChanged(qint64 duration);
    void positionChanged(qint64 position);

    void volumeChanged(int volume);
    void mutedChanged(bool muted);

public slots:
    void play();
    void pause();
    void stop();

private slots:
    void setTimeOut();
    void removeCurrentMedia();

protected:
    void setMusicEnhancedCase();

    MusicPlaylist *m_playlist;
    State m_state;
    SoundCore *m_music;
    QTimer m_timer;
    QString m_currentMedia;

};

#endif
