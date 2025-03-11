#ifndef MUSICABSTRACTSONGSLISTTABLEWIDGET_H
#define MUSICABSTRACTSONGSLISTTABLEWIDGET_H

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

#include <QMenu>
#include "musicsong.h"
#include "musicsmoothmovingwidget.h"

/*! @brief The class of the songs list abstract table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicAbstractSongsListTableWidget : public MusicSmoothMovingTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractSongsListTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicAbstractSongsListTableWidget(QWidget *parent = nullptr);
    explicit MusicAbstractSongsListTableWidget(int index, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicAbstractSongsListTableWidget();

    /*!
     * Set songs files.
     */
    virtual void setSongsList(MusicSongList *songs);
    /*!
     * Update songs files in table.
     */
    virtual void updateSongsList(const MusicSongList &songs);
    /*!
     * Select the current play row by given index.
     */
    virtual void selectRow(int index);

    /*!
     * Get all rows height.
     */
    int totalHeight() const;
    /*!
     * Set playlist row index.
     */
    void setPlaylistRow(int index) noexcept;

    /*!
     * Get the current play row.
     */
    inline void setPlayRowIndex(int index) noexcept { m_playRowIndex = index; }
    /*!
     * Get the current play row.
     */
    inline int playRowIndex() const noexcept { return m_playRowIndex; }

Q_SIGNALS:
    /*!
     * Check is current play list row.
     */
    void isCurrentPlaylistRow(bool &state);

public Q_SLOTS:
    /*!
     * Remove item from list at current row.
     */
    virtual void removeItemAt();
    /*!
     * Music item has been clicked.
     */
    void playClicked();
    /*!
     * Remove all items from list.
     */
    void clearItems();
    /*!
     * Open the music at local path.
     */
    void openFileDir();
    /*!
     * Open music file information widget.
     */
    void showFileInformation();
    /*!
     * To search song mv by song name.
     */
    void showMovieQueryWidget();
    /*!
     * Open music album query widget.
     */
    void showAlbumQueryWidget();
    /*!
     * Open music similar query widget.
     */
    void showSimilarQueryWidget();
    /*!
     * Open music song shared widget.
     */
    void showSongSharedWidget();
    /*!
     * Open music song download widget.
     */
    void showDownloadWidget();
    /*!
     * To search song mv by song name in play widget.
     */
    void showPlayedMovieQueryWidget();
    /*!
     * Open music similar query widget in play widget.
     */
    void showPlayedSimilarQueryWidget();
    /*!
     * Open music song shared widget in play widget.
     */
    void showPlayedSongSharedWidget();

protected:
    /*!
     * Create more menu information.
     */
    void createMoreMenu(QMenu *menu);
    /*!
     * Get current song path.
     */
    QString currentSongPath() const;
    /*!
     * Get song path.
     */
    QString songPath(int index) const;
    /*!
     * Get current song name.
     */
    QString currentSongName() const;
    /*!
     * Get song name.
     */
    QString songName(int index) const;

    int m_playlistRow;
    int m_playRowIndex;
    MusicSongList *m_songs;

};

#endif // MUSICABSTRACTSONGSLISTTABLEWIDGET_H
