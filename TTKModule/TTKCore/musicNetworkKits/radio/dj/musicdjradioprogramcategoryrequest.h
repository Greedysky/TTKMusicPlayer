#ifndef MUSICDJRADIOPROGRAMCATEGORYREQUEST_H
#define MUSICDJRADIOPROGRAMCATEGORYREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2022 Greedysky Studio

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

#include "musicwyqueryinterface.h"
#include "musicabstractqueryrequest.h"

/*! @brief The class of wangyi query program category download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDJRadioProgramCategoryRequest : public MusicAbstractQueryRequest, private MusicWYQueryInterface
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDJRadioProgramCategoryRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDJRadioProgramCategoryRequest(QObject *parent = nullptr);

    /*!
     * Start to Search data from name and type.
     */
    virtual void startToSearch(QueryType type, const QString &category) override final;
    /*!
     * Start to search data from name and type bt page.
     */
    virtual void startToPage(int offset) override final;
    /*!
     * Start to Search data.
     */
    void startToSearch(const QString &category);
    /*!
     * Query category info.
     */
    void queryProgramInfo(MusicResultsItem &item);

Q_SIGNALS:
    /*!
     * Create the current category item.
     */
    void createProgramItem(const MusicResultsItem &item);
    /*!
     * Create the current category info item.
     */
    void createCategoryInfoItem(const MusicResultsItem &item);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Download details data from net finished.
     */
    void downloadDetailsFinished();

};

#endif // MUSICDJRADIOPROGRAMCATEGORYREQUEST_H
