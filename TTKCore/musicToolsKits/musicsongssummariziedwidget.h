#ifndef MUSICSONGSSUMMARIZIEDWIDGET_H
#define MUSICSONGSSUMMARIZIEDWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include "musicsong.h"
#include "musicobject.h"
#include "musicsongstoolboxwidget.h"

class QTableWidgetItem;
class MusicSongsListWidget;
class MusicSongCheckToolsWidget;
class MusicSongsListFunctionWidget;

/*! @brief The class of the songs summarizied widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsSummariziedWidget : public MusicSongsToolBoxWidget
{
    Q_OBJECT
public:
    explicit MusicSongsSummariziedWidget(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongsSummariziedWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */
    bool addMusicLists(const MusicSongItems &names);
    /*!
     * Add music datas into container.
     */
    void appendMusicLists(const MusicSongItems &names);
    /*!
     * Append music datas into container.
     */
    inline const MusicSongItems& getMusicLists() const  { return m_songItems;}
    /*!
     * Get music datas from container.
     */
    void importOtherMusicSongs(QStringList &filelist);
    /*!
     * Input orther imported music datas into container.
     */
    QStringList getMusicSongsFileName(int index) const;
    /*!
     * Get music songs file name by index.
     */
    QStringList getMusicSongsFilePath(int index) const;
    /*!
     * Get music songs file path by index.
     */

    void searchFileListCache(int index, const QString &text);
    /*!
     * Set current name to searched file names to list.
     */
    bool searchFileListEmpty() const;
    /*!
     * Check current search file is empty or not.
     */
    int getSearchFileListIndex(int row);
    /*!
     * Get search file index from list by given row.
     */
    int getSearchFileListIndexAndClear(int row);
    /*!
     * Get search file index from list by given row and clear cache.
     */

    inline int getCurrentPlayToolIndex() const { return m_currentPlayToolIndex;}
    /*!
     * Get current played tool index.
     */
    void setCurrentMusicSongTreeIndex(int index);
    /*!
     * Set current music song tree index.
     */
    void playLocation(int index);
    /*!
     * Show current play index.
     */
    void selectRow(int index);
    /*!
     * Select the current play row.
     */
    void setTimerLabel(const QString &time) const;
    /*!
     * Update item time label time.
     */

Q_SIGNALS:
    void updatePlayLists(const QString &list);
    /*!
     * Update music song to lovest if in current stack widget.
     */
    void updateMediaLists(const QStringList &list, int index);
    /*!
     * Update swap the current play index if in current stack widget.
     */
    void clearSearchText();
    /*!
     * Clear current search lineedit text.
     */

public Q_SLOTS:
    void addNewRowItem();
    /*!
     * Add new play list item.
     */
    void deleteRowItem(int index);
    /*!
     * Delete selected play list item.
     */
    void deleteRowItems();
    /*!
     * Delete all other item, left than three item.
     */
    void deleteRowItemAll(int index);
    /*!
     * Delete all items in play list item.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Open rename selected play list item widget.
     */
    void addNewFiles(int index);
    /*!
     * Add new music file or files to list.
     */
    void addNewDir(int index);
    /*!
     * Add new music dir to list.
     */
    void swapDragItemIndex(int before, int after);
    /*!
     * Swap the item index by drag adn drop.
     */
    void musicImportSongsOnlyFile();
    /*!
     * Import music songs by file.
     */
    void musicImportSongsOnlyDir();
    /*!
     * Import music songs by dir.
     */
    void musicSongsCheckTestTools();
    /*!
     * Open music songs check test tools.
     */

    void setCurrentIndex();
    /*!
     * Set current play index from config file.
     */
    void musicListSongToLovestListAt(bool oper, int row);
    /*!
     * Add or remove music list song to lovest list by row.
     */
    void musicSongToLovestListAt(bool oper, int row);
    /*!
     * Add or remove music song to lovest list by row.
     */
    void addNetMusicSongToList(const QString &name, const QString &time,
                               const QString &format, bool play);
    /*!
     * Add current network music to download to local.
     */
    void addSongToPlayList(const QStringList &items);
    /*!
     * Add current selected song to play lists.
     */
    void setDeleteItemAt(const MusicObject::MIntList &del, bool fileRemove);
    /*!
     * Delete items from indexs and check remove file or not.
     */
    void setMusicIndexSwaped(int before, int after, int play, MusicSongs &songs);
    /*!
     * Swap the current play index when user drag and drop.
     */
    void isCurrentIndexs(bool &state);
    /*!
     * Check is current play stack widget.
     */
    void isSearchFileListEmpty(bool &empty);
    /*!
     * Check current list is searched or not.
     */
    void setMusicPlayCount(int index);
    /*!
     * Set current music song play count by given song index.
     */
    void setRecentMusicSongs(int index);
    /*!
     * Input recent imported music datas into container.
     */
    void getMusicLists(MusicSongItems &songs);
    /*!
     * Get music datas from container.
     */
    void updateCurrentArtist();
    /*!
     * Update current artist when it download finished.
     */
    void showFloatWidget();
    /*!
     * Show the float function widget.
     */
    void musicListSongSortBy(int index);
    /*!
     * Music list songs sort by type.
     */

private Q_SLOTS:
    void sliderValueChanaged(int value);
    /*!
     * Current vertical slider value chanaged.
     */
    void deleteFloatWidget();
    /*!
     * Delete the float function widget.
     */

protected:
    void checkCurrentNameExist(QString &name);
    /*!
     * Check current name exist.
     */
    void addNewRowItem(const QString &name);
    /*!
     * Add new play list item by name.
     */
    void createWidgetItem(MusicSongItem *item);
    /*!
     * Create widget item.
     */
    void clearAllLists();
    /*!
     * Delete all objects.
     */
    void setItemTitle(MusicSongItem *item);
    /*!
     * Set item title.
     */
    void connectMusicToolBoxWidgetItem(QObject *object);
    /*!
     * Connect music toolBox widget item.
     */
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_currentPlayToolIndex, m_searchFileListIndex;
    int m_currentImportIndex, m_currentDeleteIndex;
    bool m_toolDeleteChanged;
    MusicSongItems m_songItems;
    MusicSongsToolBoxMaskWidget *m_listMaskWidget;
    MusicObject::MIntsListMap m_searchfileListCache;
    MusicSongCheckToolsWidget *m_songCheckToolsWidget;
    MusicSongsListFunctionWidget *m_floatWidget;

};

#endif // MUSICSONGSSUMMARIZIEDWIDGET_H
