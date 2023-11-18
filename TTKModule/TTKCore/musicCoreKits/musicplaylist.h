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
    int m_playlistRow;
    QString m_path;

    MusicPlayItem()
        : m_playlistRow(-1)
    {

    }

    MusicPlayItem(int index, const QString &path)
        : m_playlistRow(index),
          m_path(path)
    {

    }

    inline bool isValid() const
    {
        return m_playlistRow != -1 && !m_path.isEmpty();
    }

    inline bool operator== (const MusicPlayItem &other) const
    {
        return m_playlistRow == other.m_playlistRow && m_path == other.m_path;
    }
};
TTK_DECLARE_LIST(MusicPlayItem);

#ifndef MusicPairItem
#define MusicPairItem std::pair<int, int>
using MusicPairItemList = QList<MusicPairItem>;
#endif

static constexpr int PLAY_NEXT_LEVEL = -123;
static constexpr int PLAY_PREVIOUS_LEVEL = -321;

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
     * Set shuffle mode.
     */
    void setShuffleMode(bool shuffle);

    /*!
     * Get current play mode.
     */
    TTK::PlayMode playbackMode() const;
    /*!
     * Set current play mode.
     */
    void setPlaybackMode(TTK::PlayMode mode);

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
    const MusicPlayItemList& mediaList() const;
    /*!
     * Get queue music media path.
     */
    const MusicPlayItemList& queueList() const;

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
     * Update item playlist row.
     */
    void update(const MusicPairItemList &indexs);

    /*!
     * Find item index at container.
     */
    int find(const MusicPlayItem &item) const;
    /*!
     * Find item by index and content.
     */
    int find(int playlistRow, const QString &content, int from = 0);

    /*!
     * Add music media, not append remember.
     */
    void add(int playlistRow, const QString &content);
    /*!
     * Add music media list, not append remember.
     */
    void add(int playlistRow, const QStringList &items);

    /*!
     * Append music medias.
     */
    void append(int playlistRow, const QString &content);
    /*!
     * Append music medias.
     */
    void append(int playlistRow, const QStringList &items);
    /*!
     * Append music media by index and content.
     */
    void appendQueue(int playlistRow, const QString &content);

    /*!
     * Remove music media from current medias by index pos.
     */
    bool remove(int pos);
    /*!
     * Remove music media from current medias by index pos.
     */
    int remove(int playlistRow, const QString &content);
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
        Shuffle();

        /*!
         * Set shuffle enable or disable.
         */
        void setEnabled(bool enable);
        /*!
         * Get shuffle enable or disable.
         */
        bool isEnabled() const;
        /*!
         * Init parameter.
        */
        void initialize(const MusicPlayItemList &items);
        /*!
         * Set current play index.
         */
        void setCurrentIndex(const MusicPlayItem &item);
        /*!
         * Set current play index.
         */
        MusicPlayItem setCurrentIndex(int index);

    private:
        int m_index;
        bool m_enable;
        MusicPlayItemList m_data;

    } m_shuffle;

};

#endif // MUSICPLAYLIST_H
