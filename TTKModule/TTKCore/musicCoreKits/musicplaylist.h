#ifndef MUSICPLAYLIST_H
#define MUSICPLAYLIST_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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
 ================================================= */

#include "musicobject.h"
#include "musicglobaldefine.h"

/*! @brief The class of the music play item.
 * @author Greedysky <greedysky@163.com>
 */
typedef struct MUSIC_CORE_EXPORT MusicPlayItem
{
    int m_toolIndex;
    QString m_path;

    MusicPlayItem()
    {
        m_toolIndex = -1;
    }

    MusicPlayItem(int index, const QString &path)
    {
        m_toolIndex = index;
        m_path = path;
    }

    bool isValid() const
    {
        return m_toolIndex != -1 && !m_path.isEmpty();
    }

    bool operator== (const MusicPlayItem &other) const
    {
        return m_toolIndex == other.m_toolIndex && m_path == other.m_path;
    }
}MusicPlayItem;
TTK_DECLARE_LISTS(MusicPlayItem)


/*! @brief The class of the music play list.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_CORE_EXPORT MusicPlaylist : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylist)
public:
    explicit MusicPlaylist(QObject *parent = nullptr);

    /*!
     * Get current play mode.
     */
    MusicObject::PlayMode playbackMode() const;
    /*!
     * Set current play mode.
     */
    void setPlaybackMode(MusicObject::PlayMode mode);

    /*!
     * Map item index at container.
     */
    int mapItemIndex(const MusicPlayItem &item) const;
    /*!
     * Get current play index.
     */
    int currentIndex() const;
    /*!
     * Get current play item.
     */
    MusicPlayItem currentItem() const;
    /*!
     * Get current play music media path.
     */
    QString currentMediaString() const;

    /*!
     * Get all music media path.
     */
    MusicPlayItems mediaListConst() const;
    /*!
     * Get all music media path.
     */
    MusicPlayItems *mediaList();
    /*!
     * Get current medias count.
     */
    int mediaCount() const;
    /*!
     * Check current medias is empty.
     */
    bool isEmpty() const;
    /*!
     * Clear current medias.
     */
    bool clear();
    /*!
     * Find item by index and content.
     */
    int find(int toolIndex, const QString &content, int from = 0);

    /*!
     * Add music media, not append remember.
     */
    void addMedia(int toolIndex, const QString &content);
    /*!
     * Add music media list, not append remember.
     */
    void addMedia(int toolIndex, const QStringList &items);
    /*!
     * Add music media list, not append remember.
     */
    void addMedia(const MusicPlayItem &item);
    /*!
     * Add music media list, not append remember.
     */
    void addMedia(const MusicPlayItems &items);

    /*!
     * Append music medias.
     */
    void appendMedia(int toolIndex, const QString &content);
    /*!
     * Append music medias.
     */
    void appendMedia(int toolIndex, const QStringList &items);
    /*!
     * Append music media.
     */
    void appendMedia(const MusicPlayItem &item);
    /*!
     * Append music medias.
     */
    void appendMedia(const MusicPlayItems &items);

    /*!
     * Get later music media path.
     */
    MusicPlayItems laterListConst() const;
    /*!
     * Insert music media by index and content.
     */
    void insertLaterMedia(int toolIndex, const QString &content);
    /*!
     * Remove music all media.
     */
    void laterListClear();

    /*!
     * Remove music media from current medias by index pos.
     */
    bool removeMedia(int pos);
    /*!
     * Remove music media from current medias by index pos.
     */
    int removeMedia(int toolIndex, const QString &content);

Q_SIGNALS:
    /*!
     * Current play index changed emit.
     */
    void currentIndexChanged(int index);
    /*!
     * Current play mode changed emit.
     */
    void playbackModeChanged(MusicObject::PlayMode mode);

public Q_SLOTS:
    /*!
     * Set current play index.
     */
    void setCurrentIndex(int index = DEFAULT_LEVEL_NORMAL);
    /*!
     * Set current play index.
     */
    void setCurrentIndex(int toolIndex, const QString &path);

protected:
    int m_currentIndex;
    MusicPlayItems m_mediaList;
    MusicPlayItems m_laterMediaList;
    MusicObject::PlayMode m_playbackMode;

};

#endif // MUSICPLAYLIST_H
