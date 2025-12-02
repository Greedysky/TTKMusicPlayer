#ifndef QMUSICPLAYLIST_H
#define QMUSICPLAYLIST_H

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

#include <QObject>
#include <QStringList>

enum SongPlayType
    {
        PlayOrder = 1,
        PlayRandom,
        PlayListLoop,
        PlayOneLoop,
        PlayOnce
    };

class MusicPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlaylist(QObject *parent = nullptr);

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
    bool removeMedia(int index);
    bool removeMedia(int start, int end);

Q_SIGNALS:
    void currentIndexChanged(int index);
    void playbackModeChanged(SongPlayType mode);
    void removeCurrentMedia();

public slots:
    void setCurrentIndex(int index = -2);
    void appendMedia(const QString &content);
    void appendMedia(const QStringList &items);
    void updateMediaLists(const QStringList &list, int index);

protected:
    SongPlayType m_playbackMode;
    QStringList m_mediaList;
    int m_currentIndex;

};

#endif
