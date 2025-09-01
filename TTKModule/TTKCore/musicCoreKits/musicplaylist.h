#ifndef MUSICPLAYLIST_H
#define MUSICPLAYLIST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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
    int m_playlistRow;
    QString m_path;

    MusicPlayItem() noexcept
        : m_playlistRow(-1)
    {

    }

    MusicPlayItem(int index, const QString &path) noexcept
        : m_playlistRow(index),
          m_path(path)
    {

    }

    inline bool isValid() const noexcept
    {
        return m_playlistRow != -1 && !m_path.isEmpty();
    }

    inline bool operator== (const MusicPlayItem &other) const noexcept
    {
        return m_playlistRow == other.m_playlistRow && m_path == other.m_path;
    }
};
TTK_DECLARE_LIST(MusicPlayItem);

static constexpr int PLAY_NEXT_LEVEL = -123;
static constexpr int PLAY_PREVIOUS_LEVEL = -321;

/*! @brief The class of the music play list.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlaylist : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlaylist(QObject *parent = nullptr);

    /*!
     * Set shuffle mode.
     */
    void setShuffleMode(bool shuffle) noexcept;

    /*!
     * Get current play mode.
     */
    TTK::PlayMode playbackMode() const noexcept;
    /*!
     * Set current play mode.
     */
    void setPlaybackMode(TTK::PlayMode mode) noexcept;

    /*!
     * Get current play index.
     */
    int currentIndex() const noexcept;
    /*!
     * Get current play item.
     */
    MusicPlayItem currentItem() const noexcept;
    /*!
     * Get current play media path.
     */
    QString currentMediaPath() const;
    /*!
     * Check current media path is same or not.
     */
    bool isSameMediaPath(const QString &path) const;

    /*!
     * Get all media path.
     */
    const MusicPlayItemList& mediaList() const noexcept;
    /*!
     * Get queue media path.
     */
    const MusicPlayItemList& queueList() const noexcept;

    /*!
     * Get current medias count.
     */
    int count() const noexcept;
    /*!
     * Check current medias is empty.
     */
    bool isEmpty() const noexcept;
    /*!
     * Clear current medias.
     */
    void clear();

    /*!
     * Update item playlist rows.
     */
    void update(const TTK::IndexPropertyList &rows);

    /*!
     * Find item index at container.
     */
    int find(const MusicPlayItem &item) const;
    /*!
     * Find item by index and content.
     */
    int find(int playlistRow, const QString &content, int from = 0);

    /*!
     * Add media, not append remember.
     */
    void add(int playlistRow, const QString &content);
    /*!
     * Add media list, not append remember.
     */
    void add(int playlistRow, const QStringList &items);

    /*!
     * Append medias.
     */
    void append(int playlistRow, const QString &content);
    /*!
     * Append medias.
     */
    void append(int playlistRow, const QStringList &items);
    /*!
     * Append media by index and content.
     */
    void appendQueue(int playlistRow, const QString &content);

    /*!
     * Remove media from current medias by index pos.
     */
    bool remove(int index);
    /*!
     * Remove media from current medias by index pos.
     */
    int remove(int playlistRow, const QString &content);
    /*!
     * Remove all queue media.
     */
    void removeQueue() noexcept;

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
    void setCurrentIndex(int index);
    /*!
     * Set current play index.
     */
    void setCurrentIndex(int playlistRow, const QString &path);

private:
    int m_currentIndex;
    MusicPlayItemList m_mediaList;
    MusicPlayItemList m_queueList;
    TTK::PlayMode m_playbackMode;

    class Shuffle
    {
    public:
        /*!
         * Object constructor.
         */
        Shuffle() noexcept;

        /*!
         * Set shuffle enable or disable.
         */
        void setEnabled(bool enable) noexcept;
        /*!
         * Get shuffle enable or disable.
         */
        bool isEnabled() const noexcept;
        /*!
         * Init parameter.
        */
        void initialize(const MusicPlayItemList &items) noexcept;
        /*!
         * Set current play index.
         */
        void setCurrentIndex(const MusicPlayItem &item) noexcept;
        /*!
         * Set current play index.
         */
        MusicPlayItem setCurrentIndex(int index) noexcept;

    private:
        int m_index;
        bool m_enable;
        MusicPlayItemList m_data;

    } m_shuffle;

};

#endif // MUSICPLAYLIST_H
