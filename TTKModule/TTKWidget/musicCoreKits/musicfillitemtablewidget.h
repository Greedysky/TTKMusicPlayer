#ifndef MUSICFILLITEMTABLEWIDGET_H
#define MUSICFILLITEMTABLEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2023 Greedysky Studio

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

#include "musicsong.h"
#include "musicabstracttablewidget.h"

class TTKCheckBoxItemDelegate;

/*! @brief The class of the fill item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFillItemTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFillItemTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicFillItemTableWidget(QWidget *parent = nullptr);
    /*!
     * Object destructor.
     */
    ~MusicFillItemTableWidget();

    /*!
     * Get checked items.
     */
    TTKIntList checkedIndexList() const;
    /*!
     * Get unchecked items.
     */
    TTKIntList uncheckedIndexList() const;

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override;
    /*!
     * Checked items state.
     */
    void checkedItemsState(bool checked);
    /*!
     * Checked items status.
     */
    void checkedItemsStatus(bool checked);

protected:
    TTKCheckBoxItemDelegate *m_checkBoxDelegate;

};

#endif // MUSICFILLITEMTABLEWIDGET_H
