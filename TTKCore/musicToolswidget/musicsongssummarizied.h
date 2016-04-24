#ifndef MUSICSONGSSUMMARIZIED_H
#define MUSICSONGSSUMMARIZIED_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QToolBox>
#include <QContextMenuEvent>
#include "musicsong.h"
#include "musicglobaldefine.h"
#include "musicobject.h"

class QTableWidgetItem;
class MusicSongsListWidget;
class MusicSongsToolItemRenamedWidget;
class MusicSongsSummariziedFloatWidget;

/*! @brief The class of the songs summarizied widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_TOOL_EXPORT MusicSongsSummarizied : public QToolBox
{
    Q_OBJECT
public:
    explicit MusicSongsSummarizied(QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongsSummarizied();

    void setMusicLists(const MusicSongsList &names);
    /*!
     * Set music datas into container.
     */
    inline const MusicSongsList& getMusicLists() const  { return m_musicFileNames;}
    /*!
     * Get music datas from container.
     */
    void importOtherMusicSongs(const QStringList &filelist);
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

    void setMusicSongsSearchedFileName(const MIntList &fileIndexs);
    /*!
     * Set current searched file indexs.
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

    inline int getCurrentPlayToolIndex() const { return m_currentIndexs;}
    /*!
     * Get current played tool index.
     */
    void setCurrentMusicSongTreeIndex(int index);
    /*!
     * Set current music song tree index.
     */
    void selectRow(int index);
    /*!
     * Select the current play row.
     */
    void setTimerLabel(const QString &time) const;
    /*!
     * Update item time label time.
     */
    void setPlaybackMode(MusicObject::SongPlayType mode) const;
    /*!
     * Set current play mode.
     */

Q_SIGNALS:
    void deleteItemAt(const MIntList &list, bool remove);
    /*!
     * Delete items from indexs if in current stack widget.
     */
    void updatePlayLists(const QString &list);
    /*!
     * Update music song to lovest if in current stack widget.
     */
    void showCurrentSong(int index);
    /*!
     * Show current song some information.
     */
    void updateMediaLists(const QStringList &list, int index);
    /*!
     * Update swap the current play index if in current stack widget.
     */
    void clearSearchText();
    /*!
     * Clear current search lineedit text.
     */
    void musicPlayIndex(int row, int col);
    /*!
     * Set current row index music to play.
     */

public Q_SLOTS:
    void addNewItem();
    /*!
     * Add new play list item.
     */
    void deleteItem();
    /*!
     * Delete selected play list item.
     */
    void changItemName();
    /*!
     * Open rename selected play list item widget.
     */
    void setCurrentIndex();
    /*!
     * Set current play index from config file.
     */
    void currentIndexChanged(int index);
    /*!
     * Current toolbox index has changed.
     */
    void addMusicSongToLovestListAt(int row);
    /*!
     * Add music song to lovest list by row.
     */
    void removeMusicSongToLovestListAt(int row);
    /*!
     * Remove music song to lovest list by row.
     */
    void addNetMusicSongToList(const QString &name, const QString &time,
                               const QString &format);
    /*!
     * Add current network music to download to local.
     */
    void addSongToPlayList(const QStringList &items);
    /*!
     * Add current selected song to play lists.
     */
    void setDeleteItemAt(const MIntList &del, bool fileRemove);
    /*!
     * Delete items from indexs and check remove file or not.
     */
    void setMusicIndexSwaped(int before, int after, int play, QStringList &list);
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
    void setChangItemName(const QString &name);
    /*!
     * Rename selected play list item.
     */
    void setTransparent(int alpha);
    /*!
     * Set background transparent.
     */
    void setMusicPlayCount(int index);
    /*!
     * Set current music song play count by given song index.
     */
    void deleteFloatWidget();
    /*!
     * Delete the float function widget.
     */
    void getMusicLists(MusicSongsList &songs, QStringList &names);
    /*!
     * Get music datas from container.
     */
    void updateCurrentArtist();
    /*!
     * Update current artist when it download finished.
     */

protected:
    void changeItemIcon();
    /*!
     * Set item icon due to the tool box index changed.
     */
    void clearAllLists();
    /*!
     * Delete all objects.
     */
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */

    int m_currentIndexs;
    int m_renameIndex;
    int m_searchFileListIndex;
    MIntsListMap m_searchfileListCache;
    MusicSongsList m_musicFileNames;
    QList<MusicSongsListWidget*> m_mainSongLists;
    MusicSongsToolItemRenamedWidget *m_renameLine;
    MusicSongsSummariziedFloatWidget *m_floatWidget;

};

#endif // MUSICSONGSSUMMARIZIED_H
