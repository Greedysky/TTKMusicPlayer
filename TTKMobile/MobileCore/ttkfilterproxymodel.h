#ifndef TTKFILTERPROXYMODEL_H
#define TTKFILTERPROXYMODEL_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (c) 2014 - 2016 Greedysky Studio
 * All rights reserved!
 * Redistribution and use of the source code or any derivative
 * works are strictly forbiden.
   =================================================*/

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
