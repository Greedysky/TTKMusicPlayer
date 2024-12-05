#ifndef MUSICPLAYEDLISTPOPWIDGET_H
#define MUSICPLAYEDLISTPOPWIDGET_H

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

#include <QLabel>
#include "musicsong.h"
#include "musictoolmenuwidget.h"

class QScrollArea;
class MusicPlaylist;
class MusicSongsListPlayedTableWidget;

/*! @brief The class of the played list top container widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlayedListTopContainerWidget : public QFrame
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlayedListTopContainerWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlayedListTopContainerWidget(QWidget *parent = nullptr);

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

};


/*! @brief The class of the played list pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlayedListPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlayedListPopWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicPlayedListPopWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicPlayedListPopWidget();

    /*!
     * Get class object instance.
     */
    static MusicPlayedListPopWidget *instance();

    /*!
     * Set current play list.
     */
    void setPlaylist(MusicPlaylist *playlist);

    /*!
     * Clear music data list.
     */
    void clear();
    /*!
     * Clear play queue state.
     */
    void clearQueueState();
    /*!
     * Update current played item list.
     */
    void updatePlayedList(const TTK::IndexPropertyList &indexs);

    /*!
     * Remove music from data list.
     */
    void remove(int index);
    /*!
     * Remove music from data list.
     */
    void remove(int playlistRow, const QString &path);
    /*!
     * Remove music from data list.
     */
    void remove(int playlistRow, const MusicSong &song);
    /*!
     * Remove music from data list.
     */
    void remove(int playlistRow, const MusicSongList &songs);

    /*!
     * Append music to data list.
     */
    void append(const MusicSongList &songs);
    /*!
     * Append music to data list.
     */
    void append(int playlistRow, const MusicSong &song);
    /*!
     * Append music to data list.
     */
    void append(int playlistRow, const MusicSongList &songs);

    /*!
     * Insert music after played music index.
     */
    void insert(int playlistRow, const MusicSong &song);
    /*!
     * Insert music by given index.
     */
    void insert(int playlistRow, const MusicSongList &songs);
    /*!
     * Insert music by given index.
     */
    void insert(int playlistRow, int index, const MusicSong &song);
    /*!
     * Insert music by given index.
     */
    void insert(int playlistRow, int index, const MusicSongList &song);

    /*!
     * Set current select played music index.
     */
    void selectCurrentIndex();
    /*!
     * Set current select played music index.
     */
    void selectCurrentIndex(int playlistRow, const MusicSong &song);

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    virtual void popupMenu() override final;
    /*!
     * Remove items from indexs.
     */
    void removeItemAt(const TTKIntList &index);
    /*!
     * Remove all items from list.
     */
    void clearItems();
    /*!
     * List cell item double clicked.
     */
    void itemDoubleClicked();
    /*!
     * List cell double clicked.
     */
    void itemDoubleClicked(int row, int column);

private:
    /*!
     * Create all widget in layout.
     */
    void initialize();
    /*!
     * Create container widget.
     */
    QWidget *createContainerWidget();
    /*!
     * Set play list songs files.
     */
    void setPlaylistSongs();
    /*!
     * Set play list count.
     */
    void setPlaylistCount(int count);
    /*!
     * Set play state to pause or stop.
     */
    void clearPlaylist();

    QList<QLabel*> m_labels;
    MusicPlaylist *m_playlist;
    MusicSongList m_songList;
    QScrollArea *m_scrollArea;
    MusicSongsListPlayedTableWidget *m_tableWidget;

    static MusicPlayedListPopWidget *m_instance;
};

#endif // MUSICPLAYEDLISTPOPWIDGET_H
