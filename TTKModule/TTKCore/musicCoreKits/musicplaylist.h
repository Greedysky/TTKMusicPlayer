#ifndef MUSICPLAYLIST_H
#define MUSICPLAYLIST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicglobaldefine.h"

/*! @brief The class of the music play item.
 * @author Greedysky <greedysky@163.com>
 */
struct TTK_MODULE_EXPORT MusicPlayItem
{
    int m_toolIndex;
    QString m_path;

    MusicPlayItem()
        : m_toolIndex(-1)
    {

    }

    MusicPlayItem(int index, const QString &path)
        : m_toolIndex(index),
          m_path(path)
    {

    }

    inline bool isValid() const
    {
        return m_toolIndex != -1 && !m_path.isEmpty();
    }

    inline bool operator== (const MusicPlayItem &other) const
    {
        return m_toolIndex == other.m_toolIndex && m_path == other.m_path;
    }
};
TTK_DECLARE_LIST(MusicPlayItem);

#define PLAY_NEXT_LEVEL               -123
#define PLAY_PREVIOUS_LEVEL           -321

void playNext();
/*! @brief The class of the music play list.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylist : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlaylist)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylist(QObject *parent = nullptr);

    /*!
     * Get current play mode.
     */
    TTK::PlayMode playbackMode() const;
    /*!
     * Set current play mode.
     */
    void setPlaybackMode(TTK::PlayMode mode);

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
    QString currentMediaPath() const;

    /*!
     * Get all music media path.
     */
    MusicPlayItemList *mediaList();
    /*!
     * Get queue music media path.
     */
    MusicPlayItemList *queueList();

    /*!
     * Get current medias count.
     */
    int count() const;
    /*!
     * Check current medias is empty.
     */
    bool isEmpty() const;
    /*!
     * Clear current medias.
     */
    void clear();
    /*!
     * Find item by index and content.
     */
    int find(int toolIndex, const QString &content, int from = 0);

    /*!
     * Add music media, not append remember.
     */
    void add(int toolIndex, const QString &content);
    /*!
     * Add music media list, not append remember.
     */
    void add(int toolIndex, const QStringList &items);

    /*!
     * Append music medias.
     */
    void append(int toolIndex, const QString &content);
    /*!
     * Append music medias.
     */
    void append(int toolIndex, const QStringList &items);
    /*!
     * Append music media by index and content.
     */
    void appendQueue(int toolIndex, const QString &content);

    /*!
     * Remove music media from current medias by index pos.
     */
    bool remove(int pos);
    /*!
     * Remove music media from current medias by index pos.
     */
    int remove(int toolIndex, const QString &content);
    /*!
     * Remove music all queue media.
     */
    void removeQueue();

Q_SIGNALS:
    /*!
     * Current play index changed.
     */
    void currentIndexChanged(int index);
    /*!
     * Current play mode changed.
     */
    void playbackModeChanged(TTK::PlayMode mode);

public Q_SLOTS:
    /*!
     * Set current play index.
     */
    void setCurrentIndex(int index = TTK_LOW_LEVEL);
    /*!
     * Set current play index.
     */
    void setCurrentIndex(int toolIndex, const QString &path);

private:
    int m_currentIndex;
    MusicPlayItemList m_mediaList;
    MusicPlayItemList m_queueList;
    TTK::PlayMode m_playbackMode;

};

#endif // MUSICPLAYLIST_H
