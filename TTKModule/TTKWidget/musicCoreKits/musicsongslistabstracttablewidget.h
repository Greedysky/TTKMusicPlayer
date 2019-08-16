#ifndef MUSICSONGSLISTABSTRACTTABLEWIDGET_H
#define MUSICSONGSLISTABSTRACTTABLEWIDGET_H

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

#include <QMenu>
#include "musicsong.h"
#include "musicsmoothmovingwidget.h"

/*! @brief The class of the songs list abstract table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListAbstractTableWidget : public MusicSmoothMovingTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsListAbstractTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsListAbstractTableWidget(QWidget *parent = nullptr);

    virtual ~MusicSongsListAbstractTableWidget();

    /*!
     * Set songs file names.
     */
    virtual void setSongsFileName(MusicSongs *songs);
    /*!
     * Update songs file names in table.
     */
    virtual void updateSongsFileName(const MusicSongs &songs);
    /*!
     * Select the current play row by given index.
     */
    virtual void selectRow(int index);

    /*!
     * Get all rows height.
     */
    int allRowsHeight() const;
    /*!
     * Set parent tool index.
     */
    void setParentToolIndex(int index);
    /*!
     * Get the current play row.
     */
    inline void setPlayRowIndex(int index) { m_playRowIndex = index; }
    /*!
     * Get the current play row.
     */
    inline int getPlayRowIndex() const { return m_playRowIndex; }

Q_SIGNALS:
    /*!
     * Check is current play stack widget.
     */
    void isCurrentIndex(bool &state);

public Q_SLOTS:
    /*!
     * Music item has been clicked.
     */
    void musicPlayClicked();
    /*!
     * Delete item from list at current row.
     */
    virtual void setDeleteItemAt();
    /*!
     * Delete all items from list.
     */
    void setDeleteItemAll();
    /*!
     * Open the music at local path.
     */
    void musicOpenFileDir();
    /*!
     * Open music file information widget.
     */
    void musicFileInformation();
    /*!
     * To search song mv by song name.
     */
    void musicSongMovieFound();
    /*!
     * Open music album found widget.
     */
    void musicAlbumFoundWidget();
    /*!
     * Open music similar found widget.
     */
    void musicSimilarFoundWidget();
    /*!
     * Open music song shared widget.
     */
    void musicSongSharedWidget();
    /*!
     * Open music song download widget.
     */
    void musicSongDownload();
    /*!
     * To search song mv by song name in play widget.
     */
    void musicSongPlayedMovieFound();
    /*!
     * Open music similar found widget in play widget.
     */
    void musicPlayedSimilarFoundWidget();
    /*!
     * Open music song shared widget in play widget.
     */
    void musicSongPlayedSharedWidget();
    /*!
     * Open music song KMicro widget in play widget.
     */
    void musicSongPlayedKMicroWidget();

protected:
    /*!
     * Create more menu information.
     */
    void createMoreMenu(QMenu *menu);
    /*!
     * Get current song path.
     */
    QString getCurrentSongPath() const;
    /*!
     * Get song path.
     */
    QString getSongPath(int index) const;
    /*!
     * Get current song name.
     */
    QString getCurrentSongName() const;
    /*!
     * Get song name.
     */
    QString getSongName(int index) const;

    int m_playRowIndex, m_parentToolIndex;
    MusicSongs *m_musicSongs;
    bool m_hasParentToolIndex;

};

#endif // MUSICSONGSLISTABSTRACTTABLEWIDGET_H
