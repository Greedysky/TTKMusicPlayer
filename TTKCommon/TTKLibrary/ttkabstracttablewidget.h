#ifndef TTKABSTRACTTABLEWIDGET_H
#define TTKABSTRACTTABLEWIDGET_H

/***************************************************************************
 * This file is part of the TTK Library Module project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#include <QScrollBar>
#include <QHeaderView>
#include <QTableWidget>
#include "ttkitemdelegate.h"

/*! @brief The class of the ttk abstract table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT TTKAbstractTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit TTKAbstractTableWidget(QWidget *parent = nullptr);

    /*!
     * Get selected item rows.
     */
    TTKIntList selectedRows() const;

public Q_SLOTS:
    /*!
     * Table widget item cell enter.
     */
    virtual void itemCellEntered(int row, int column);
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column);
    /*!
     * Remove all items.
     */
    virtual void removeItems();

protected:
    /*!
     * Override the widget event.
     */
    virtual void leaveEvent(QEvent *event) override;

protected:
    /*!
     * Set selected item row color.
     */
    void setRowColor(int row, const QColor &color) const;
    /*!
     * Check item row is valid or not.
     */
    inline bool isValid() const noexcept
    {
        return !(rowCount() == 0 || currentRow() < 0);
    }

    int m_previousColorRow;
    int m_previousClickRow;
    QColor m_backgroundColor;

};

#endif // TTKABSTRACTTABLEWIDGET_H
