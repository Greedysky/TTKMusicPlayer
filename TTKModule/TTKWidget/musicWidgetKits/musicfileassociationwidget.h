#ifndef MUSICFILEASSOCIATIONWIDGET_H
#define MUSICFILEASSOCIATIONWIDGET_H

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

#include "musicabstractmovedialog.h"
#include "musicfillitemtablewidget.h"

namespace Ui {
class MusicFileAssociationWidget;
}


/*! @brief The class of the file association item selected table widget
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFileAssociationTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFileAssociationTableWidget(QWidget *parent = nullptr);

    /*!
     * Save data items.
     */
    void save() const;

public Q_SLOTS:
    /*!
     * Table widget item cell click.
     */
    virtual void itemCellClicked(int row, int column) override;
    /*!
     * Checked items status.
     */
    void checkedItemsStatus(bool checked);

private:
    /*!
     * Create cell items by input data.
     */
    void addCellItems();

};


/*! @brief The class of the file association widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFileAssociationWidget : public MusicAbstractMoveDialog
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFileAssociationWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    /*!
     * Confirm button clicked.
     */
    void confirmButtonClicked();

private:
    Ui::MusicFileAssociationWidget *m_ui;

};


/*! @brief The namespace of the association helper.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    /*!
     * Get current support association files.
     */
    TTK_MODULE_EXPORT QStringList supportAssociations();
    /*!
     * Get current unsupport association files.
     */
    TTK_MODULE_EXPORT QStringList unsupportAssociations();

}


#endif // MUSICFILEASSOCIATIONWIDGET_H
