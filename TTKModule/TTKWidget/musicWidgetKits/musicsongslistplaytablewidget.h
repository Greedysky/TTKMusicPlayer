#ifndef MUSICSONGSLISTPLAYTABLEWIDGET_H
#define MUSICSONGSLISTPLAYTABLEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2024 Greedysky Studio

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

#include <QTimer>
#include "musicabstractsongslisttablewidget.h"

class QPropertyAnimation;
class MusicOpenFileWidget;
class MusicSongsListPlayWidget;
class MusicSongsListItemInfoWidget;
class MusicLineEditItemDelegate;

/*! @brief The class of the songs list play table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsListPlayTableWidget : public MusicAbstractSongsListTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongsListPlayTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsListPlayTableWidget(int index, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsListPlayTableWidget();

    /*!
     * Update songs files in table.
     */
    virtual void updateSongsList(const MusicSongList &songs) override final;
    /*!
     * Select the current play row.
     */
    virtual void selectRow(int index) override final;

    /*!
     * Set current search result indexs.
     */
    void updateSearchFileName(MusicSongList *songs, const TTKIntList &result);

    /*!
     * Set music sort.
     */
    inline void setSongSort(MusicSongSort *sort) { m_songSort = sort; }
    /*!
     * Update item time label time.
     */
    void updateTimeLabel(const QString &current, const QString &total) const;
    /*!
     * Update current artist when it download finished.
     */
    void updateCurrentArtist();
    /*!
     * Adjust current play widget row.
     */
    void adjustPlayWidgetRow();
    /*!
     * Create upload file module.
     */
    bool createUploadFileModule();

Q_SIGNALS:
    /*!
     * Check current list is searched or not.
     */
    void isSearchedResultEmpty(bool &empty);
    /*!
     * Delete items from indexs and check remove file or not.
     */
    void deleteItemAt(const TTKIntList &index, bool fileRemove);
    /*!
     * Swap the current play index when user drag and drop.
     */
    void itemIndexSwaped(int start, int end, int play, MusicSongList &songs);
    /*!
     * Add or remove music list song to lovest list by row.
     */
    void addSongToLovestList(bool state, int row);
    /*!
     * Show the float function widget.
     */
    void showFloatWidget();
    /*!
     * Music list songs sort by type.
     */
    void songListSortBy(int index);

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override final;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Remove item from list at current row.
     */
    virtual void removeItemAt() override final;
    /*!
     * Remove item or items from list with file.
     */
    void removeItemWithFile();
    /*!
     * Show play item information widget.
     */
    void showTimeOut();
    /*!
     * Hide play item information widget.
     */
    void stayTimeOut();
    /*!
     * Reset item name by current row.
     */
    void setChangSongName();
    /*!
     * Open music make ring widget.
     */
    void showMakeRingWidget();
    /*!
     * Open music transform widget.
     */
    void showTransformWidget();
    /*!
     * Open music song search query.
     */
    void searchQueryByName(QAction *action);
    /*!
     * Add music to played list and play later.
     */
    void addToPlayLater();
    /*!
     * Add music to played list.
     */
    void addToPlayedList();
    /*!
     * Rename item artist label is finised.
     */
    void itemRenameFinished(const QString &name);
    /*!
     * Music list songs sort by type.
     */
    void songListSortBy(QAction *action);

private:
    /*!
     * Override the widget event.
     */
    virtual void mousePressEvent(QMouseEvent *event) override final;
    virtual void mouseMoveEvent(QMouseEvent *event) override final;
    virtual void mouseReleaseEvent(QMouseEvent *event) override final;
    virtual void leaveEvent(QEvent *event) override final;
    virtual void wheelEvent(QWheelEvent *event) override final;
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    /*!
     * Close rename item.
     */
    void closeRenameItem();
    /*!
     * Start to drag to play list.
     */
    void startToDrag();

    QWidget *m_parent;
    int m_dragStartIndex;
    QPoint m_dragStartPoint;
    bool m_mouseMoved;

    QTimer m_timerShow, m_timerStay;
    MusicOpenFileWidget *m_openFileWidget;
    MusicSongsListItemInfoWidget *m_songsInfoWidget;
    MusicSongsListPlayWidget *m_songsPlayWidget;

    bool m_leftButtonPressed;
    bool m_renameActived, m_deleteItemWithFile;
    MusicSongList m_searchedSongs;
    QTableWidgetItem *m_renameItem;
    MusicLineEditItemDelegate *m_renameEditDelegate;
    MusicSongSort *m_songSort;

};

#endif // MUSICSONGSLISTPLAYTABLEWIDGET_H
