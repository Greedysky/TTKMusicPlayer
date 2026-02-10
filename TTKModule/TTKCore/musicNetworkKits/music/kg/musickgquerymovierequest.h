#ifndef MUSICKGQUERYMOVIEREQUEST_H
#define MUSICKGQUERYMOVIEREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2026 Greedysky Studio

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

#include "musickgqueryinterface.h"
#include "musicunityquerymovierequest.h"

/*! @brief The class of the kugou query mv data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKGQueryMovieRequest : public MusicUnityQueryMovieRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicKGQueryMovieRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by offset page.
     */
    virtual void startToPage(int offset) override final;
    /*!
     * Start to search data by input data.
     */
    virtual void startToSearch(const QString &value) override final;
    /*!
     * Start to search data by input value.
     */
    virtual void startToSearchByID(const QString &value) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downloadFinished() override final;

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downloadSingleFinished();

};


/*! @brief The class of the kugou query artist mv data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKGQueryArtistMovieRequest : public MusicQueryMovieRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicKGQueryArtistMovieRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by offset page.
     */
    virtual void startToPage(int offset) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downloadFinished() override final;

};

#endif // MUSICKGQUERYMOVIEREQUEST_H
