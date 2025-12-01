#ifndef MUSICSONGSCONTAINERWIDGET_H
#define MUSICSONGSCONTAINERWIDGET_H

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

#include "musicsearchinterface.h"
#include "musicsongstoolboxwidget.h"
#include "musicsongsearchonlinewidget.h"

class MusicSongSearchDialog;
class MusicSongsListWidget;
class MusicSongsListFunctionWidget;

/*! @brief The class of the songs container widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsContainerWidget : public MusicSongsToolBoxWidget, private MusicItemSearchInterface<MusicSongItemList>
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsContainerWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsContainerWidget();

    /*!
     * Get class object instance.
     */
    static MusicSongsContainerWidget *instance();

    /*!
     * Update media datas from container.
     */
    void updateSongItem(const MusicSongItem &item);
    /*!
     * Add media datas into container.
     */
    bool addSongItemList(const MusicSongItemList &items);
    /*!
     * Append media datas into container.
     */
    void appendSongItemList(const MusicSongItemList &items);
    /*!
     * Query media datas from container.
     */
    inline const MusicSongItemList& items() const noexcept { return m_containerItems; }

    /*!
     * Input imported media urls into container.
     */
    void importSongsByUrl(const QString &path, int playlistRow);
    /*!
     * Input imported media datas into container.
     */
    void importSongsByPath(const QStringList &files, int playlistRow);

    /*!
     * Get media songs file name by index.
     */
    QStringList songsFileName(int index) const;
    /*!
     * Get media songs file path by index.
     */
    QStringList songsFilePath(int index) const;
    /*!
     * Map media song index by file path.
     */
    int mapSongIndexByFilePath(int playlistRow, const QString &path) const;
    /*!
     * Map media file path by song index.
     */
    QString mapFilePathBySongIndex(int playlistRow, int index) const;

    /*!
     * Get search result index from list by given row and clear cache.
     */
    void removeSearchResult(int &row);

    /*!
     * Get current play row index.
     */
    inline int playRowIndex() const noexcept { return m_playRowIndex; }
    /*!
     * Set current song play index.
     */
    void setPlayRowIndex(int index);

    /*!
     * Show current play index.
     */
    void playLocation(int index);
    /*!
     * Select the current play row.
     */
    void selectRow(int index);
    /*!
     * Update item duration label.
     */
    void updateDurationLabel(const QString &current, const QString &total) const;

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
    void deleteAllItems(int index);
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int start, int end);
    /*!
     * Add media to played list and play later.
     */
    void addToPlayLater(int index);
    /*!
     * Add media to played list.
     */
    void addToPlayedList(int index);
    /*!
     * Import media songs by files.
     */
    void importSongsByFiles(int index = TTK_LOW_LEVEL);
    /*!
     * Import media songs by dir.
     */
    void importSongsByDir(int index = TTK_LOW_LEVEL);
    /*!
     * Open media songs check test tools.
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
     * Add or remove media song to lovest list by row.
     */
    void songToLovestListAt(bool state, int row);
    /*!
     * Add or remove media list song to lovest list by row.
     */
    void addSongToLovestList(bool state, int row);
    /*!
     * Add current network media buffer to playlist.
     */
    void addSongBufferToPlaylist(const MusicResultDataItem &item);
    /*!
     * Add current selected song to play list.
     */
    void addSongToPlaylist(const QStringList &items);
    /*!
     * Remove items from rows and check remove file or not.
     */
    void removeItemAt(const TTKIntList &rows, bool fileRemove);
    /*!
     * Swap the current play index when user drag and drop.
     */
    void itemRowSwaped(int start, int end, int play, MusicSongList &songs);
    /*!
     * Check is current play stack widget.
     */
    void isCurrentPlaylistRow(bool &state);
    /*!
     * Check current list is searched or not.
     */
    void isSearchedResultEmpty(bool &empty);
    /*!
     * Set current media song play count by given song index.
     */
    void setSongPlayCount(int index);
    /*!
     * Input recent imported media datas into container.
     */
    void appendRecentSongs(int index);
    /*!
     * Query media datas from container.
     */
    void querySongItemList(MusicSongItemList &songs);
    /*!
     * Update current artist when it download finished.
     */
    void updateCurrentArtist();
    /*!
     * Show the float function widget.
     */
    void showFloatWidget();
    /*!
     * Media list songs sort by type.
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
    void deleteFunctionWidget();

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
    inline bool isSearchedPlayIndex() const noexcept { return m_lastSearchIndex == m_currentIndex; }
    /*!
     * Close searched text widget.
     */
    void closeSearchWidget();
    /*!
     * Close searched text widget in need.
     */
    void closeSearchWidgetInNeed();
    /*!
     * Check title name valid.
     */
    void checkTitleNameValid(QString &name);
    /*!
     * Create widget item.
     */
    void createWidgetItem(MusicSongItem *item);
    /*!
     * Create container item.
     */
    MusicSongItem* createContainerItem(const MusicSongItem &item);
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
     * Update current played rows.
     */
    void updatePlayedRows(int start, int end);
    /*!
     * Input imported media datas into container.
     */
    void importSongsWithProgress(QWidget *progress, int offset, const QStringList &files, int playlistRow);

    int m_playRowIndex;
    int m_lastSearchIndex;
    int m_selectDeleteIndex;

    MusicSongsToolBoxMaskWidget *m_topMaskWidget;
    MusicSongsListFunctionWidget *m_functionWidget;
    MusicSongSearchDialog *m_songSearchWidget;

    static MusicSongsContainerWidget *m_instance;

};

#endif // MUSICSONGSCONTAINERWIDGET_H
