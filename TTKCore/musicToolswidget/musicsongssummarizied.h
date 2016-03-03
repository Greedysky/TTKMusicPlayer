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

class MusicSongsListWidget;
class QTableWidgetItem;
class MusicSongsToolItemRenamedWidget;

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
    void setMusicSongsSearchedFileName(const MIntList &fileIndexs);
    void searchFileListCache(int index, const QString &text);
    bool searchFileListEmpty() const;
    int getsearchFileListIndex(int row);
    int getsearchFileListIndexAndClear(int row);
    void importOtherMusicSongs(const QStringList &filelist);

    QStringList getMusicSongsFileName(int index) const;
    QStringList getMusicSongsFilePath(int index) const;
    inline const MusicSongsList& getMusicLists() const  { return m_musicFileNames;}

    inline int getCurrentPlayToolIndex() const { return m_currentIndexs;}
    void currentMusicSongTreeIndexChanged(int index);
    void selectRow(int index);
    void setTimerLabel(const QString &time) const;
    /*!
     * Update item time label time.
     */
    void setPlaybackMode(MusicObject::SongPlayType mode) const;

Q_SIGNALS:
    void deleteItemAt(MIntList list);
    /*!
     * Delete items from indexs if in current stack widget.
     */
    void updatePlayLists(const QString &list);
    /*!
     * Update music song to lovest if in current stack widget.
     */
    void showCurrentSong(int index);
    void updateMediaLists(const QStringList &list, int index);
    /*!
     * Update swap the current play index if in current stack widget.
     */
    void clearSearchText();
    void musicPlayIndex(int row, int col);

public Q_SLOTS:
    void addNewItem();
    void deleteItem();
    void changItemName();
    void setCurrentIndex();
    void currentIndexChanged(int index);
    void addMusicSongToLovestListAt(int row);
    /*!
     * Add music song to lovest list by row.
     */
    void addNetMusicSongToList(const QString &name, const QString &time,
                               const QString &format);
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
    void setTransparent(int alpha);
    /*!
     * Set background transparent.
     */
    void setMusicPlayCount(int index);

protected:
    void changeItemIcon();
    void clearAllLists();
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

};

#endif // MUSICSONGSSUMMARIZIED_H
