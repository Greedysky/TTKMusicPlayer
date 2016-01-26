#ifndef QMUSICPLAYLIST_H
#define QMUSICPLAYLIST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QStringList>
#include <QObject>

enum SongPlayType
    {
        MC_PlayOrder = 1,
        MC_PlayRandom,
        MC_PlayListLoop,
        MC_PlayOneLoop,
        MC_PlayOnce
    };

class MusicPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlaylist(QObject *parent = 0);
    ~MusicPlaylist() = default;

    SongPlayType playbackMode() const;
    void setPlaybackMode(SongPlayType mode);

    int currentIndex() const;
    QString currentMediaString() const;
    int mediaCount() const;
    bool isEmpty() const;
    bool clear();

    void addMedia(const QString &content);
    void addMedia(const QStringList &items);
    bool insertMedia(int index, const QString &content);
    bool insertMedia(int index, const QStringList &items);
    bool removeMedia(int pos);
    bool removeMedia(int start, int end);

Q_SIGNALS:
    void currentIndexChanged(int index);
    void playbackModeChanged(SongPlayType mode);
    void removeCurrentMedia();

public Q_SLOTS:
    void setCurrentIndex(int index = -2);
    void appendMedia(const QString &content);
    void appendMedia(const QStringList &items);
    void updateMediaLists(const QStringList &list, int index);

protected:
    SongPlayType m_playbackMode;
    QStringList m_mediaList;
    int m_currentIndex;

};

#endif // QMUSICPLAYLIST_H
