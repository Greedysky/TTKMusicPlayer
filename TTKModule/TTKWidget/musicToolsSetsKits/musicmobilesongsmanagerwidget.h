#ifndef MUSICMOBILESONGSMANAGERWIDGET_H
#define MUSICMOBILESONGSMANAGERWIDGET_H

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

#include "musicwidgetheaders.h"
#include "musicsearchinterface.h"
#include "musicabstractmovewidget.h"
#include "musicabstractsongslisttablewidget.h"

/*! @brief The class of the mobile songs table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMobileSongsTableWidget : public MusicAbstractSongsListTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicMobileSongsTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicMobileSongsTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicMobileSongsTableWidget();

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const QStringList &songs);

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column) override final;
    /*!
     * Remove all items.
     */
    virtual void removeItems() override final;
    /*!
     * Override the widget event.
     */
    virtual void contextMenuEvent(QContextMenuEvent *event) override final;

};

class MusicSongsManagerThread;

namespace Ui {
class MusicMobileSongsManagerWidget;
}

/*! @brief The class of the mobile songs manager widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicMobileSongsManagerWidget : public MusicAbstractMoveWidget, private MusicItemSearchInterface<QStringList>
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicMobileSongsManagerWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicMobileSongsManagerWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicMobileSongsManagerWidget();

    /*!
     * Find extra device path.
     */
    void findExtraDevicePath(const QString &dir);

public Q_SLOTS:
    /*!
     * Select all items.
     */
    void selectedAllItems(bool checked);
    /*!
     * Media song audition play.
     */
    void auditionButtonClick();
    /*!
     * Item cell on click by row and col.
     */
    void itemCellClicked(int row, int column);
    /*!
     * Item cell on double click by row and col.
     */
    void itemDoubleClicked(int row, int column);
    /*!
     * Send the searched file or path.
     */
    void searchFilePathChanged(const QStringList &path);
    /*!
     * Search result from list.
     */
    void searchResultChanged(int row, int column);

private:
    /*!
     * Clear All Items.
     */
    void clearItems();
    /*!
     * Set current item or items selected.
     */
    void selectedItemsToPlaylist();

    Ui::MusicMobileSongsManagerWidget *m_ui;

    MusicSongsManagerThread *m_thread;

};

#endif // MUSICMOBILESONGSMANAGERWIDGET_H
