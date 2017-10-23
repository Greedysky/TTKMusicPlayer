#ifndef TTKFILTERPROXYMODEL_H
#define TTKFILTERPROXYMODEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2017 Greedysky Studio

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

#include <QSortFilterProxyModel>
#include "musicmobileglobaldefine.h"

/*! @brief The class of the filter proxy model.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_MOBILE_EXPORT TTKFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit TTKFilterProxyModel(QObject *parent = 0);
    /*!
     * Object contsructor.
     */

    Q_INVOKABLE void sourceModel(QAbstractItemModel *sourceModel);
    /*!
     * Set the source model.
     */
    Q_INVOKABLE void sortColumn(int column, int order);
    /*!
     * Set the source model sort in column.
     */

};

#endif // TTKFILTERPROXYMODEL_H
