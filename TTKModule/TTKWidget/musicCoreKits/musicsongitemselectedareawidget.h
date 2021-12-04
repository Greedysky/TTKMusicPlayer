#ifndef MUSICSONGITEMSELECTEDAREAWIDGET_H
#define MUSICSONGITEMSELECTEDAREAWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2021 Greedysky Studio

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
     * Object contsructor.
     */
    explicit MusicSongItemSelectedTableWidget(QWidget *parent = nullptr);

    /*!
     * Create all items.
     */
    void createAllItems(MusicSongItems *items);
    /*!
     * Get selected items.
     */
    TTKIntList selectedItems() const;

public Q_SLOTS:
    /*!
     * Select all items.
     */
    void selectedAllItems(bool check);

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
     * Object contsructor.
     */
    explicit MusicSongItemSelectedDialog(QWidget *parent = nullptr);
    ~MusicSongItemSelectedDialog();

    /*!
     * Create all items.
     */
    void createAllItems(MusicSongItems *items);

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
    /*!
     * Override exec function.
     */
    virtual int exec();

protected:
    Ui::MusicSongItemSelectedDialog *m_ui;

};


class MusicClickedLabel;

/*! @brief The class of the song item selected area widget
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicSongItemSelectedAreaWidget : public QWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicSongItemSelectedAreaWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicSongItemSelectedAreaWidget(QWidget *parent = nullptr);
    ~MusicSongItemSelectedAreaWidget();

    /*!
     * Get selected song items.
     */
    MusicSongs selectedSongItems();

Q_SIGNALS:
    /*!
     * Query music datas from container.
     */
    void queryMusicItemList(MusicSongItems &songs);
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

protected:
    bool m_selected;
    TTKIntList m_selectedItems;

    QLabel *m_label, *m_itemLabel;
    MusicClickedLabel *m_modifiedItemButton;
};

#endif // MUSICSONGITEMSELECTEDAREAWIDGET_H
