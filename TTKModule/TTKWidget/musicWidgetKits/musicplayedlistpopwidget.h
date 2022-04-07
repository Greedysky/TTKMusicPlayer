#ifndef MUSICPLAYEDLISTPOPWIDGET_H
#define MUSICPLAYEDLISTPOPWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#define MakePlayedItem(a, b) std::pair<int, int>(a, b);
typedef QList< std::pair<int, int> > PlayedItemList;


/*! @brief The class of the played list top container widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicPlayedListTopContainerWidget : public QFrame
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlayedListTopContainerWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlayedListTopContainerWidget(QWidget *parent = nullptr);

protected:
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
     * Object contsructor.
     */
    explicit MusicPlayedListPopWidget(QWidget *parent = nullptr);
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
     * Get current play list.
     */
    MusicPlaylist *playlist() const;

    /*!
     * Clear music data list.
     */
    void clear();
    /*!
     * Get current played item list.
     */
    void resetToolIndex(const PlayedItemList &indexs);

    /*!
     * Remove music from data list.
     */
    void remove(int index);
    /*!
     * Remove music from data list.
     */
    void remove(int toolIndex, const QString &path);
    /*!
     * Remove music from data list.
     */
    void remove(int toolIndex, const MusicSong &song);

    /*!
     * Append music to data list.
     */
    void append(int toolIndex, const MusicSong &song);
    /*!
     * Append music to data list.
     */
    void append(const MusicSongList &song);

    /*!
     * Insert music after played music index.
     */
    void insert(int toolIndex, const MusicSong &song);
    /*!
     * Insert music by given index.
     */
    void insert(int toolIndex, int index, const MusicSong &song);

    /*!
     * Set current select played music index.
     */
    void setCurrentIndex();
    /*!
     * Set current select played music index.
     */
    void setCurrentIndex(int toolIndex, const MusicSong &song);

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    virtual void popupMenu() override final;
    /*!
     * Delete items from indexs.
     */
    void setDeleteItemAt(const TTKIntList &index);
    /*!
     * Delete all items from list.
     */
    void setDeleteItemAll();
    /*!
     * List cell item double clicked.
     */
    void itemDoubleClicked();
    /*!
     * List cell double clicked.
     */
    void itemDoubleClicked(int row, int column);

protected:
    /*!
     * Create all widget in layout.
     */
    void initialize();
    /*!
     * Create container widget.
     */
    QWidget *createContainerWidget();
    /*!
     * Update songs file names.
     */
    void updateSongsFileName();
    /*!
     * Set play list count.
     */
    void setPlaylistCount(int count);
    /*!
     * Set play state to pause or stop.
     */
    void setPlaylistEmpty();

    QList<QLabel*> m_labels;
    MusicPlaylist *m_playlist;
    MusicSongList m_songList;
    QScrollArea *m_scrollArea;
    MusicSongsListPlayedTableWidget *m_playedListWidget;

    static MusicPlayedListPopWidget *m_instance;
};

#endif // MUSICPLAYEDLISTPOPWIDGET_H
