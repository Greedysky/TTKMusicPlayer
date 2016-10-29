#ifndef TTKMUSICPLAYLIST_H
#define TTKMUSICPLAYLIST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicmobileglobaldefine.h"

/*! @brief The class of the music playlist.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKMusicPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit TTKMusicPlaylist(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    int playbackMode() const;
    /*!
     * Get current play mode.
     */
    void setPlaybackMode(int mode);
    /*!
     * Set current play mode.
     */

    void addMedia(const QString &content);
    /*!
     * Add music media, not append remember.
     */
    void addMedia(const QStringList &items);
    /*!
     * Add music media list, not append remember.
     */
    void removeMedia(const QString &content);
    /*!
     * Remove music media by value.
     */
    void removeMedia(int index);
    /*!
     * Remove music media by index.
     */

    QString currentMediaString() const;
    /*!
     * Get current play music media path.
     */
    int currentIndex() const;
    /*!
     * Get current play index.
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

    void playNext();
    /*!
     * Set current player to next music.
     */
    void playPrevious();
    /*!
     * Set current player to previous music.
     */

Q_SIGNALS:
    void currentIndexChanged(int index);
    /*!
     * Current play index changed emit.
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

protected:
    int m_currentIndex;
    int m_playbackMode;
    QStringList m_mediaList;

};

#endif // TTKMUSICPLAYLIST_H
