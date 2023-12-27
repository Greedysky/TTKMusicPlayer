#ifndef MUSICSONGITEMSELECTEDAREAWIDGET_H
#define MUSICSONGITEMSELECTEDAREAWIDGET_H

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

#include "musicabstractmovedialog.h"
#include "musicfillitemtablewidget.h"

/*! @brief The class of the song item selected table widget
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongItemSelectedTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongItemSelectedTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongItemSelectedTableWidget(QWidget *parent = nullptr);

    /*!
     * Create cell items by input data.
     */
    void addCellItems(MusicSongItemList *items);
    /*!
     * Get checked data items.
     */
    TTKIntList checkedDataList() const;

};


namespace Ui {
class MusicSongItemSelectedDialog;
}

/*! @brief The class of the song item selected dialog widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongItemSelectedDialog : public MusicAbstractMoveDialog
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongItemSelectedDialog)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongItemSelectedDialog(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongItemSelectedDialog();

    /*!
     * Create cell items by input data.
     */
    void addCellItems(MusicSongItemList *items);

Q_SIGNALS:
    /*!
     * Item list changed.
     */
    void itemListChanged(const TTKIntList &items);

public Q_SLOTS:
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();

private:
    Ui::MusicSongItemSelectedDialog *m_ui;

};


class TTKClickedLabel;

/*! @brief The class of the song item selected area widget
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongItemSelectedAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongItemSelectedAreaWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicSongItemSelectedAreaWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicSongItemSelectedAreaWidget();

    /*!
     * Get selected song items.
     */
    MusicSongList selectedSongItems();

Q_SIGNALS:
    /*!
     * Query music datas from container.
     */
    void queryMusicItemList(MusicSongItemList &songs);
    /*!
     * Confirm changed.
     */
    void confirmChanged();

public Q_SLOTS:
    /*!
     * Modified item button clicked.
     */
    void modifiedItemButtonClicked();
    /*!
     * Item list changed.
     */
    void itemListChanged(const TTKIntList &items);

private:
    bool m_selected;
    TTKIntList m_items;

    QLabel *m_label, *m_itemLabel;
    TTKClickedLabel *m_modifiedItemButton;

};

#endif // MUSICSONGITEMSELECTEDAREAWIDGET_H
