#ifndef MUSICCONNECTTRANSFERTABLEWIDGET_H
#define MUSICCONNECTTRANSFERTABLEWIDGET_H

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

#include "musicfillitemtablewidget.h"

/*! @brief The class of the transfer file list table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicConnectTransferTableWidget : public MusicFillItemTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicConnectTransferTableWidget(QWidget *parent = nullptr);

    /*!
     * Create cell items by input data.
     */
    void addCellItems(const MusicSongList &songs);

};


/*! @brief The class of the transfer file list complete table widget.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicConnectTransferCompleteTableWidget : public MusicAbstractTableWidget
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicConnectTransferCompleteTableWidget(QWidget *parent = nullptr);

public Q_SLOTS:
    /*!
     * Create cell item by input data.
     */
    void addCellItem(const QString &name);

};

#endif // MUSICCONNECTTRANSFERTABLEWIDGET_H
