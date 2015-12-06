#ifndef MUSICCOREMPLAYER_H
#define MUSICCOREMPLAYER_H

/* =================================================
 * This file is part of the Music Player project
 * Copyright (c) 2014 - 2015 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
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

    enum Category
    {
        NullCategory,
        MusicCategory,
        RadioCategory,
        VideoCategory
    };

    explicit MusicCoreMPlayer(QObject *parent = 0);
    ~MusicCoreMPlayer();

    void setMedia(Category type, const QString &data, int winId);
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

    void radioChanged();
    void musicChanged();

public slots:
    void play();
    void stop();

private slots:
    void dataRecieve();
    void positionRecieve();
    void durationRecieve();
    void radioStandardRecieve();
    void musicStandardRecieve();
    void timeout();

protected:
    void setVideoMedia(const QString &data, int winId);
    void setMusicMedia(const QString &data);
    void setRadioMedia(const QString &data);

    QProcess *m_process;
    State m_playState;
    Category m_category;
    QTimer m_timer;
    int m_currentPos;

};

#endif // MUSICCOREMPLAYER_H
