#ifndef MUSICABSTRACTTABLEWIDGET_H
#define MUSICABSTRACTTABLEWIDGET_H

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

#include <QTableWidget>
#include <QHeaderView>

#include "musicglobaldefine.h"
#include "musicobject.h"
#include "musicuiobject.h"
#include "musicwidgetutils.h"

#define ITEM_ROW_HEIGHT_S   25
#define ITEM_ROW_HEIGHT_M   30
#define ITEM_ROW_HEIGHT_L   40
#define ITEM_ROW_HEIGHT_XL  60

/*! @brief The class of the table widget base.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_WIDGET_EXPORT MusicAbstractTableWidget : public QTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicAbstractTableWidget)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicAbstractTableWidget(QWidget *parent = nullptr);

    virtual ~MusicAbstractTableWidget();

    /*!
     * Clear current items.
     */
    void clear();

public Q_SLOTS:
    /*!
     * Table widget list cell enter.
     */
    virtual void listCellEntered(int row, int column);
    /*!
     * Table widget list cell click.
     * Subclass should implement this function.
     */
    virtual void listCellClicked(int row, int column) = 0;

protected:
    /*!
     * Set selected item row color.
     */
    void setRowColor(int row, const QColor &color) const;
    /*!
     * Get multi index set.
     */
    MIntList getMultiIndexSet();

    QColor m_defaultBkColor;
    int m_previousColorRow;
    int m_previousClickRow;

};

#endif // MUSICABSTRACTTABLEWIDGET_H
