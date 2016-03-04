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
#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the music playlist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit MusicPlaylist(QObject *parent = 0);
    /*!
     * Object contsructor.
     */
    ~MusicPlaylist() = default;

    MusicObject::SongPlayType playbackMode() const;
    /*!
     * Get current play mode.
     */
    void setPlaybackMode(MusicObject::SongPlayType mode);
    /*!
     * Set current play mode.
     */

    int currentIndex() const;
    /*!
     * Get current play index.
     */
    QString currentMediaString() const;
    /*!
     * Get current play music media path.
     */
    int mediaCount() const;
    /*!
     * Get current medias count.
     */
    bool isEmpty() const;
    /*!
     * Check current medias is empty.
     */
    bool clear();
    /*!
     * Clear current medias.
     */

    void addMedia(const QString &content);
    /*!
     * Add music media, not append remember.
     */
    void addMedia(const QStringList &items);
    /*!
     * Add music media list, not append remember.
     */
    bool insertMedia(int index, const QString &content);
    /*!
     * Insert music media into current medias by index.
     */
    bool insertMedia(int index, const QStringList &items);
    /*!
     * Insert music medias into current medias by index.
     */
    bool removeMedia(int pos);
    /*!
     * Remove music media from current medias by index.
     */
    bool removeMedia(int start, int end);
    /*!
     * Remove music media from current medias by index between start and end.
     */

Q_SIGNALS:
    void currentIndexChanged(int index);
    /*!
     * Current play index changed emit.
     */
    void playbackModeChanged(MusicObject::SongPlayType mode);
    /*!
     * Current play mode changed emit.
     */

public Q_SLOTS:
    void setCurrentIndex(int index = -2);
    /*!
     * Set current play index.
     */
    void appendMedia(const QString &content);
    /*!
     * Append music media.
     */
    void appendMedia(const QStringList &items);
    /*!
     * Append music medias.
     */
    void updateMediaLists(const QStringList &list, int index);
    /*!
     * Update music medias into current medias by index.
     */

protected:
    MusicObject::SongPlayType m_playbackMode;
    QStringList m_mediaList;
    int m_currentIndex;

};

#endif // QMUSICPLAYLIST_H
