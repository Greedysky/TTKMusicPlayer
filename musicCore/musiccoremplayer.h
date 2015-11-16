#ifndef MUSICCOREMPLAYER_H
#define MUSICCOREMPLAYER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QObject>
#include "musiclibexportglobal.h"

class QProcess;

class MUSIC_CORE_EXPORT MusicCoreMPlayer : public QObject
{
    Q_OBJECT
public:
    enum State
    {
        StoppedState,
        PlayingState,
        PausedState
    };

    explicit MusicCoreMPlayer(QObject *parent = 0);
    ~MusicCoreMPlayer();

    void setMedia(const QString &data, int winId);
    void setPosition(qint64 pos);

    void setMute(bool mute);
    void setVolume(int value);

    inline State state() { return m_playState;}

signals:
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void volumnChanged(int volumn);
    void mediaChanged(const QString &data);
    void stateChanged(State state);

public slots:
    void play();
    void stop();

private slots:
    void positionRecieve();
    void durationRecieve();

protected:
    QProcess *m_process;
    State m_playState;

};

#endif // MUSICCOREMPLAYER_H
