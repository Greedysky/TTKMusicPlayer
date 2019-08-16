#ifndef MUSICPLAYEDLISTPOPWIDGET_H
#define MUSICPLAYEDLISTPOPWIDGET_H

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

#include "musicsong.h"
#include "musictoolmenuwidget.h"

class QLabel;
class QScrollArea;
class MusicPlaylist;
class MusicSongsListPlayedTableWidget;

#define PlayedPairItem(a, b) std::pair<int, int>(a, b);
typedef QList< std::pair<int, int> > PlayedPairList;


/*! @brief The class of the played list top container widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlayedListTopContainerWidget : public QWidget
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
    virtual void paintEvent(QPaintEvent *event) override;

};


/*! @brief The class of the played list pop widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicPlayedListPopWidget : public MusicToolMenuWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicPlayedListPopWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicPlayedListPopWidget(QWidget *parent = nullptr);

    virtual ~MusicPlayedListPopWidget();

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
     * Get current played list.
     */
    void resetToolIndex(const PlayedPairList &indexs);

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
    void append(const MusicSongs &song);

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
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();

public Q_SLOTS:
    /*!
     * To popup menu.
     */
    void popupMenu();
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemAt(int index);
    /*!
     * Delete all items from list.
     */
    void setDeleteItemAll();
    /*!
     * List cell double clicked.
     */
    void cellDoubleClicked(int row, int column);

protected:
    /*!
     * Create all widget in layout.
     */
    void initWidget();
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
    MusicSongs m_songLists;
    QScrollArea *m_scrollArea;
    MusicSongsListPlayedTableWidget *m_playedListWidget;

    static MusicPlayedListPopWidget *m_instance;
};

#endif // MUSICPLAYEDLISTPOPWIDGET_H
