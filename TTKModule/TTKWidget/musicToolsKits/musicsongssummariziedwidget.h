#ifndef MUSICSONGSSUMMARIZIEDWIDGET_H
#define MUSICSONGSSUMMARIZIEDWIDGET_H

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

#include "musicsearchinterface.h"
#include "musicsongstoolboxwidget.h"
#include "musicsongsearchonlinewidget.h"

class QTableWidgetItem;
class MusicSongsListWidget;
class MusicSongsListFunctionWidget;
class MusicSongSearchDialog;

/*! @brief The class of the songs summarizied widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsSummariziedWidget : public MusicSongsToolBoxWidget, private MusicItemSearchInterface<MusicSongItemList>
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsSummariziedWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsSummariziedWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsSummariziedWidget();

    /*!
     * Add music datas into container.
     */
    bool addMusicItemList(const MusicSongItemList &items);
    /*!
     * Append music datas into container.
     */
    void appendMusicItemList(const MusicSongItemList &items);
    /*!
     * Query music datas from container.
     */
    inline const MusicSongItemList& musicItemList() const { return m_containerItems; }

    /*!
     * Input imported music urls into container.
     */
    void importMusicSongsByUrl(const QString &path, int playlistRow);
    /*!
     * Input imported music datas into container.
     */
    void importMusicSongsByPath(const QStringList &files, int playlistRow);

    /*!
     * Get music songs file name by index.
     */
    QStringList musicSongsFileName(int index) const;
    /*!
     * Get music songs file path by index.
     */
    QStringList musicSongsFilePath(int index) const;
    /*!
     * Map music song index by file path.
     */
    int mapSongIndexByFilePath(int playlistRow, const QString &path) const;
    /*!
     * Map music file path by song index.
     */
    QString mapFilePathBySongIndex(int playlistRow, int index) const;

    /*!
     * Get search result index from list by given row and clear cache.
     */
    void removeSearchResult(int &row);

    /*!
     * Get current play row index.
     */
    inline int playRowIndex() const { return m_playRowIndex; }
    /*!
     * Set current song tree index.
     */
    void setCurrentSongTreeIndex(int index);

    /*!
     * Show current play index.
     */
    void playLocation(int index);
    /*!
     * Select the current play row.
     */
    void selectRow(int index);
    /*!
     * Update item time label time.
     */
    void updateTimeLabel(const QString &current, const QString &total) const;

public Q_SLOTS:
    /*!
     * Add new play list item.
     */
    void addNewRowItem();
    /*!
     * Delete selected play list item.
     */
    void deleteRowItem(int index);
    /*!
     * Delete all other item, left than three item.
     */
    void deleteRowItems();
    /*!
     * Delete all items in play list item.
     */
    void deleteRowItemAll(int index);
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int start, int end);
    /*!
     * Add music to played list and play later.
     */
    void addToPlayLater(int index);
    /*!
     * Add music to played list.
     */
    void addToPlayedList(int index);
    /*!
     * Import music songs by files.
     */
    void importSongsByFiles(int index = TTK_LOW_LEVEL);
    /*!
     * Import music songs by dir.
     */
    void importSongsByDir(int index = TTK_LOW_LEVEL);
    /*!
     * Open music songs check test tools.
     */
    void showSongCheckToolsWidget();
    /*!
     * Open lrc batch download.
     */
    void showLrcDownloadBatchWidget();
    /*!
     * Search result from list.
     */
    void searchResultChanged(int row, int column);

    /*!
     * Update current play index from config file.
     */
    void updateCurrentIndex();
    /*!
     * Add or remove music song to lovest list by row.
     */
    void songToLovestListAt(bool state, int row);
    /*!
     * Add or remove music list song to lovest list by row.
     */
    void addSongToLovestList(bool state, int row);
    /*!
     * Add current network music buffer to playlist.
     */
    void addSongBufferToPlaylist(const MusicResultDataItem &songItem);
    /*!
     * Add current selected song to play list.
     */
    void addSongToPlaylist(const QStringList &items);
    /*!
     * Remove items from indexs and check remove file or not.
     */
    void removeItemAt(const TTKIntList &del, bool fileRemove);
    /*!
     * Swap the current play index when user drag and drop.
     */
    void itemIndexSwaped(int start, int end, int play, MusicSongList &songs);
    /*!
     * Check is current play stack widget.
     */
    void isCurrentPlaylistRow(bool &state);
    /*!
     * Check current list is searched or not.
     */
    void isSearchedResultEmpty(bool &empty);
    /*!
     * Set current music song play count by given song index.
     */
    void setMusicPlayCount(int index);
    /*!
     * Input recent imported music datas into container.
     */
    void setRecentMusicSongs(int index);
    /*!
     * Query music datas from container.
     */
    void queryMusicItemList(MusicSongItemList &songs);
    /*!
     * Update current artist when it download finished.
     */
    void updateCurrentArtist();
    /*!
     * Show the float function widget.
     */
    void showFloatWidget();
    /*!
     * Music list songs sort by type.
     */
    void songListSortBy(int index);

private Q_SLOTS:
    /*!
     * Show searched text widget.
     */
    void showSearchWidget();
    /*!
     * Current vertical slider value chanaged.
     */
    void sliderValueChanaged(int value);
    /*!
     * Delete the float function widget.
     */
    void deleteFloatWidget();

private:
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override final;
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    virtual void dragEnterEvent(QDragEnterEvent *event) override final;
    virtual void dragMoveEvent(QDragMoveEvent *event) override final;
    virtual void dropEvent(QDropEvent *event) override final;
    /*!
     * Current index is searched or not.
     */
    inline bool isSearchPlayIndex() const { return m_lastSearchIndex == m_currentIndex; }
    /*!
     * Close searched text widget.
     */
    void closeSearchWidget();
    /*!
     * Close searched text widget in need.
     */
    void closeSearchWidgetInNeed();
    /*!
     * Check current name exist.
     */
    void checkCurrentNameExist(QString &name);
    /*!
     * Add new play list item by name.
     */
    void addNewRowItem(const QString &name);
    /*!
     * Create widget item.
     */
    void createWidgetItem(MusicSongItem *item);
    /*!
     * Set item title.
     */
    void setItemTitle(MusicSongItem *item);
    /*!
     * Set input connection.
     */
    void setInputModule(QObject *object) const;
    /*!
     * Resize window bound by resize called.
     */
    void resizeWindow();
    /*!
     * Update current played list.
     */
    void updatePlayedList(int start, int end);

    int m_playRowIndex;
    int m_lastSearchIndex;
    int m_selectDeleteIndex;
    bool m_toolDeleteChanged;

    MusicSongsToolBoxMaskWidget *m_listMaskWidget;
    MusicSongsListFunctionWidget *m_listFunctionWidget;
    MusicSongSearchDialog *m_songSearchWidget;

};

#endif // MUSICSONGSSUMMARIZIEDWIDGET_H
