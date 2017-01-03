#ifndef MUSICSONGSLISTTABLEWIDGET_H
#define MUSICSONGSLISTTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2015 - 2017 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

#include <QTimer>
#include "musicsongslistabstracttablewidget.h"

class QPropertyAnimation;
class MusicOpenFileWidget;
class MusicSongsListPlayWidget;
class MusicSongsListItemInfoWidget;

/*! @brief The class of the songs list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicSongsListTableWidget : public MusicSongsListAbstractTableWidget
{
    Q_OBJECT
public:
    explicit MusicSongsListTableWidget(int index, QWidget *parent = 0);
    /*!
     * Object contsructor.
     */
    virtual ~MusicSongsListTableWidget();

    static QString getClassName();
    /*!
     * Get class object name.
     */

    virtual void updateSongsFileName(const MusicSongs &songs) override;
    /*!
     * Update songs file names in table.
     */
    void clearAllItems();
    /*!
     * Clear All Items.
     */

    int allRowsHeight() const;
    /*!
     * Get all rows height.
     */
    virtual void selectRow(int index) override;
    /*!
     * Select the current play row.
     */
    inline int getPlayRowIndex() const { return m_playRowIndex; }
    /*!
     * Get the current play row.
     */
    inline void setPlaybackMode(MusicObject::SongPlayType type) { m_songplaymode = type;}
    /*!
     * Set current play mode.
     */
    inline int getTransparent() const { return m_transparent;}
    /*!
     * Get item transparent.
     */
    inline void setTransparent(int transparent) { m_transparent = transparent;}
    /*!
     * Set item transparent.
     */
    void setTimerLabel(const QString &t) const;
    /*!
     * Update item time label time.
     */
    void updateCurrentArtist();
    /*!
     * Update current artist when it download finished.
     */
    void replacePlayWidgetRow();
    /*!
     * Replace current play widget row.
     */
    bool createUploadFileWidget();
    /*!
     * Create upload file widget.
     */

Q_SIGNALS:
    void musicPlayOrder();
    /*!
     * Set current play mdoe to order.
     */
    void musicPlayRandom();
    /*!
     * Set current play mdoe to random.
     */
    void musicPlayListLoop();
    /*!
     * Set current play mdoe to list loop.
     */
    void musicPlayOneLoop();
    /*!
     * Set current play mdoe to single loop.
     */
    void musicPlayItemOnce();
    /*!
     * Set current play mdoe to just once.
     */
    void musicAddNewFiles();
    /*!
     * Add new music file or files to list.
     */
    void musicAddNewDir();
    /*!
     * Add new music dir to list.
     */

    void isSearchFileListEmpty(bool &empty);
    /*!
     * Check current list is searched or not.
     */
    void deleteItemAt(const MusicObject::MIntList &index, bool fileRemove);
    /*!
     * Delete items from indexs and check remove file or not.
     */
    void getMusicIndexSwaped(int before, int after, int play, MusicSongs &songs);
    /*!
     * Swap the current play index when user drag and drop.
     */
    void musicListSongToLovestListAt(bool oper, int row);
    /*!
     * Add or remove music list song to lovest list by row.
     */
    void showFloatWidget();
    /*!
     * Show the float function widget.
     */

public Q_SLOTS:
    virtual void listCellEntered(int row, int column) override;
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Table widget list cell click.
     */
    virtual void setDeleteItemAt() override;
    /*!
     * Delete item from list at current row.
     */
    void setDeleteItemWithFile();
    /*!
     * Delete item or items from list with file.
     */
    void showTimeOut();
    /*!
     * Show play item information widget.
     */
    void stayTimeOut();
    /*!
     * Hide play item information widget.
     */
    void setChangSongName();
    /*!
     * Reset item name by current row.
     */
    void musicMakeRingWidget();
    /*!
     * Open music make ring widget.
     */
    void musicTransformWidget();
    /*!
     * Open music transform widget.
     */
    void musicSongMovieFoundPy();
    /*!
     * To search song mv by song name in play widget.
     */
    void musicSimilarFoundWidgetPy();
    /*!
     * Open music similar found widget in play widget.
     */
    void musicSongSharedWidgetPy();
    /*!
     * Open music song shared widget in play widget.
     */
    void musicSearchQuery(QAction *action);
    /*!
     * Open music song search query.
     */
    void musicAddToPlayLater();
    /*!
     * Add music to played list and play later.
     */
    void musicAddToPlayedList();
    /*!
     * Add music to played list.
     */
    void setItemRenameFinished(const QString &name);
    /*!
     * Rename item artist label is finised.
     */

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    /*!
     * Override the widget event.
     */
    void closeRenameItem();
    /*!
     * Close rename item.
     */
    void startToDrag();
    /*!
     * Start to drag to play list.
     */
    void createContextMenu(QMenu &menu);
    /*!
     * Create context menu.
     */

    int m_transparent, m_dragStartIndex;
    QPoint m_dragStartPoint;
    bool m_mouseMoved;

    QTimer m_timerShow, m_timerStay;
    MusicOpenFileWidget *m_openFileWidget;
    MusicSongsListItemInfoWidget *m_musicSongsInfoWidget;
    MusicSongsListPlayWidget *m_musicSongsPlayWidget;

    bool m_leftButtonPressed;
    bool m_renameActived, m_deleteItemWithFile;
    QTableWidgetItem *m_renameItem;
    MusicObject::SongPlayType m_songplaymode;

};

#endif // MUSICSONGSLISTTABLEWIDGET_H
