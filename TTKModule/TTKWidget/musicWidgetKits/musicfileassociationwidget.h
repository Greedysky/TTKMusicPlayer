#ifndef MUSICFILEASSOCIATIONWIDGET_H
#define MUSICFILEASSOCIATIONWIDGET_H

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

#include "musicabstractmovedialog.h"
#include "musicfillitemtablewidget.h"

namespace Ui {
class MusicFileAssociationWidget;
}


/*! @brief The class of the song item selected table widget
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFileAssociationTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicFileAssociationTableWidget)
public:
    /*!
     * Object constructor.
     */
    explicit MusicFileAssociationTableWidget(QWidget *parent = nullptr);

    /*!
     * Get unchecked data items.
     */
    void uncheckedDataList() const;

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
    TTK_DECLARE_MODULE(MusicFileAssociationWidget)
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

#endif // MUSICFILEASSOCIATIONWIDGET_H
