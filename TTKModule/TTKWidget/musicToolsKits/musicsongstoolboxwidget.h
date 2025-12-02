#ifndef MUSICSONGSTOOLBOXWIDGET_H
#define MUSICSONGSTOOLBOXWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musicfunctiontoolboxwidget.h"

struct MusicSongSort;
class MusicItemRenameEidt;

/*! @brief The class of the tool box top widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsToolBoxTopWidget : public MusicFunctionToolBoxTopWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsToolBoxTopWidget(int index, const QString &text, QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsToolBoxTopWidget();

    /*!
     * Set media sort.
     */
    inline void setSongSort(MusicSongSort *sort) noexcept { m_songSort = sort; }

Q_SIGNALS:
    /*!
     * Add new play list item.
     */
    void addNewRowItem();
    /*!
     * Delete selected play list item.
     */
    void deleteRowItem(int index);
    /*!
     * Delete all items in play list item.
     */
    void deleteAllItems(int index);
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Add new media file or files to list.
     */
    void addNewFiles(int index);
    /*!
     * Add new media dir to list.
     */
    void addNewDir(int index);
    /*!
     * Media list songs sort by type.
     */
    void songListSortBy(int index);
    /*!
     * Add media to played list and play later.
     */
    void addToPlayLater(int index);
    /*!
     * Add media to played list.
     */
    void addToPlayedList(int index);

public Q_SLOTS:
    /*!
     * Delete selected play list item.
     */
    void deleteRowItemChanged();
    /*!
     * Delete all items in play list item.
     */
    void deleteAllItemsChanged();
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemNameChanged();
    /*!
     * Rename item artist label is finised.
     */
    void changItemName(const QString &name);
    /*!
     * Add new media file or files to list.
     */
    void addNewFilesChanged();
    /*!
     * Add new media dir to list.
     */
    void addNewDirChanged();
    /*!
     * Export media songs by item.
     */
    void exportSongsItem();
    /*!
     * Show menu items.
     */
    void showMenu();
    /*!
     * Media list songs sort by type.
     */
    void songListSortBy(QAction *action);
    /*!
     * Add media to played list and play later.
     */
    void addToPlayLater();
    /*!
     * Add media to played list.
     */
    void addToPlayedList();

private:
    /*!
     * Check the item is enable or not.
     */
    virtual bool isItemEnabled() const override final;
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;
    virtual void paintEvent(QPaintEvent *event) override;

    MusicSongSort *m_songSort;
    MusicItemRenameEidt *m_renameEdit;

};


/*! @brief The class of the tool box mask widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsToolBoxMaskWidget : public MusicSongsToolBoxTopWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsToolBoxMaskWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongsToolBoxMaskWidget();

private:
    /*!
     * Override the widget event.
     */
    virtual void paintEvent(QPaintEvent *event) override final;

    QTimer *m_updateTimer;

};


/*! @brief The class of the tool box widget item.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsToolBoxWidgetItem : public MusicFunctionToolBoxWidgetItem
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsToolBoxWidgetItem(int index, const QString &text, QWidget *parent = nullptr);

    /*!
     * Set media sort.
     */
    void setSongSort(MusicSongSort *sort);

Q_SIGNALS:
    /*!
     * Add new play list item.
     */
    void addNewRowItem();
    /*!
     * Delete selected play list item.
     */
    void deleteRowItem(int index);
    /*!
     * Delete all items in play list item.
     */
    void deleteAllItems(int index);
    /*!
     * Open rename selected play list item widget.
     */
    void changRowItemName(int index, const QString &name);
    /*!
     * Add new media file or files to list.
     */
    void addNewFiles(int index);
    /*!
     * Add new media dir to list.
     */
    void addNewDir(int index);
    /*!
     * Media list songs sort by type.
     */
    void songListSortBy(int index);
    /*!
     * Add media to played list and play later.
     */
    void addToPlayLater(int index);
    /*!
     * Add media to played list.
     */
    void addToPlayedList(int index);

};


/*! @brief The class of the tool box widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongsToolBoxWidget : public MusicFunctionToolBoxWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongsToolBoxWidget(QWidget *parent = nullptr);

    /*!
     * Set media sort.
     */
    void setSongSort(QWidget *item, MusicSongSort *sort);

    /*!
     * Get current valid index.
     */
    int makeValidIndex() const;

private:
    /*!
     * Create item.
     */
    virtual MusicFunctionToolBoxWidgetItem* initialItem(QWidget *item, const QString &text) override final;

};

#endif // MUSICSONGSTOOLBOXWIDGET_H
