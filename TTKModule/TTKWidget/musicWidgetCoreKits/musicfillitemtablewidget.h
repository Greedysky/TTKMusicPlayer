#ifndef MUSICFILLITEMTABLEWIDGET_H
#define MUSICFILLITEMTABLEWIDGET_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "musicabstracttablewidget.h"

class MusicQueryTableDelegate;

/*! @brief The class of the fill item table widget.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicFillItemTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFillItemTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicFillItemTableWidget(QWidget *parent = nullptr);

    virtual ~MusicFillItemTableWidget();

    /*!
     * Get selected items.
     */
    MIntList getSelectedItems() const;

public Q_SLOTS:
    /*!
     * Table widget list cell click.
     */
    virtual void listCellClicked(int row, int column) override;
    /*!
     * Set select all items.
     */
    void setSelectedAllItems(bool all);

protected:
    MusicQueryTableDelegate *m_checkBoxDelegate;

};

#endif // MUSICFILLITEMTABLEWIDGET_H
