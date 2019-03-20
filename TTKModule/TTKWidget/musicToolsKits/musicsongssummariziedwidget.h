#ifndef MUSICSONGSSUMMARIZIEDWIDGET_H
#define MUSICSONGSSUMMARIZIEDWIDGET_H

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
#include "musicobject.h"
#include "musicsongstoolboxwidget.h"

class QTableWidgetItem;
class MusicSongsListWidget;
class MusicSongCheckToolsWidget;
class MusicSongsListFunctionWidget;
class MusicLocalSongSearchDialog;
class MusicLrcDownloadBatchWidget;

/*! @brief The class of the songs summarizied widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsSummariziedWidget : public MusicSongsToolBoxWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsSummariziedWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongsSummariziedWidget(QWidget *parent = nullptr);

    virtual ~MusicSongsSummariziedWidget();

    /*!
     * Add music datas into container.
     */
    bool addMusicLists(const MusicSongItems &names);
    /*!
     * Append music datas into container.
     */
    void appendMusicLists(const MusicSongItems &names);
    /*!
     * Get music datas from container.
     */
    inline const MusicSongItems& getMusicLists() const  { return m_songItems; }
    /*!
     * Input orther imported music datas into container.
     */
    void importOtherMusicSongs(QStringList &filelist);
    /*!
     * Get music songs file name by index.
     */
    QStringList getMusicSongsFileName(int index) const;
    /*!
     * Get music songs file path by index.
     */
    QStringList getMusicSongsFilePath(int index) const;
    /*!
     * Map music song index by file path.
     */
    int mapSongIndexByFilePath(int toolIndex, const QString &path) const;
    /*!
     * Map music file path by song index.
     */
    QString mapFilePathBySongIndex(int toolIndex, int index) const;

    /*!
     * Set current name to searched file names to list.
     */
    void searchFileListCache(int index);
    /*!
     * Check current search file is empty or not.
     */
    bool searchFileListEmpty() const;
    /*!
     * Get search file index from list by given row.
     */
    int getSearchFileListIndex(int row);
    /*!
     * Get search file index from list by given row and clear cache.
     */
    int getSearchFileListIndexAndClear(int row);

    /*!
     * Get current played tool index.
     */
    inline int getCurrentPlayToolIndex() const { return m_currentPlayToolIndex;}
    /*!
     * Set current music song tree index.
     */
    void setCurrentMusicSongTreeIndex(int index);
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
     * Add new music file or files to list.
     */
    void addNewFiles(int index);
    /*!
     * Add new music dir to list.
     */
    void addNewDir(int index);
    /*!
     * Swap the item index by drag adn drop.
     */
    void swapDragItemIndex(int before, int after);
    /*!
     * Add music to played list and play later.
     */
    void addToPlayLater(int index);
    /*!
     * Add music to played list.
     */
    void addToPlayedList(int index);
    /*!
     * Import music songs by file.
     */
    void musicImportSongsOnlyFile();
    /*!
     * Import music songs by dir.
     */
    void musicImportSongsOnlyDir();
    /*!
     * Open music songs check test tools.
     */
    void musicSongsCheckTestTools();
    /*!
     * Open lrc batch download.
     */
    void musicLrcBatchDownload();

    /*!
     * Set current play index from config file.
     */
    void setCurrentIndex();
    /*!
     * Add or remove music list song to lovest list by row.
     */
    void musicListSongToLovestListAt(bool oper, int row);
    /*!
     * Add or remove music song to lovest list by row.
     */
    void musicSongToLovestListAt(bool oper, int row);
    /*!
     * Add current network music to download to local.
     */
    void addNetMusicSongToList(const QString &name, const QString &time, const QString &format, bool play);
    /*!
     * Add current selected song to play lists.
     */
    void addSongToPlaylist(const QStringList &items);
    /*!
     * Delete items from indexs and check remove file or not.
     */
    void setDeleteItemAt(const MIntList &del, bool fileRemove);
    /*!
     * Swap the current play index when user drag and drop.
     */
    void setMusicIndexSwaped(int before, int after, int play, MusicSongs &songs);
    /*!
     * Check is current play stack widget.
     */
    void isCurrentIndex(bool &state);
    /*!
     * Check current list is searched or not.
     */
    void isSearchFileListEmpty(bool &empty);
    /*!
     * Set current music song play count by given song index.
     */
    void setMusicPlayCount(int index);
    /*!
     * Input recent imported music datas into container.
     */
    void setRecentMusicSongs(int index);
    /*!
     * Get music datas from container.
     */
    void getMusicLists(MusicSongItems &songs);
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
    void musicListSongSortBy(int index);

private Q_SLOTS:
    /*!
     * Show searched text widget.
     */
    void musicSearchWidget();
    /*!
     * Current vertical slider value chanaged.
     */
    void sliderValueChanaged(int value);
    /*!
     * Delete the float function widget.
     */
    void deleteFloatWidget();

protected:
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
     * Delete all objects.
     */
    void clearAllLists();
    /*!
     * Set item title.
     */
    void setItemTitle(MusicSongItem *item);
    /*!
     * Connect music toolBox widget item.
     */
    void connectMusicToolBoxWidgetItem(QObject *object);
    /*!
     * Resize window bound by widgte resize called.
     */
    void resizeWindow();
    /*!
     * Get current played list.
     */
    void resetToolIndex();
    /*!
     * Override the widget event.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;

    int m_currentPlayToolIndex, m_searchFileListIndex;
    int m_currentImportIndex, m_currentDeleteIndex;
    bool m_toolDeleteChanged;

    MusicSongItems m_songItems;
    MusicSongsToolBoxMaskWidget *m_listMaskWidget;
    MIntListMap m_searchfileListCache;
    MusicSongCheckToolsWidget *m_songCheckToolsWidget;
    MusicSongsListFunctionWidget *m_listFunctionWidget;
    MusicLocalSongSearchDialog *m_musicSongSearchWidget;
    MusicLrcDownloadBatchWidget *m_lrcBatchDownloadWidget;

};

#endif // MUSICSONGSSUMMARIZIEDWIDGET_H
