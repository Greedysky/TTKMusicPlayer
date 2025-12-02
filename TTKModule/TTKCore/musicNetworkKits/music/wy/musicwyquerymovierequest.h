#ifndef MUSICWYQUERYMOVIEREQUEST_H
#define MUSICWYQUERYMOVIEREQUEST_H

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

#include "musicwyqueryinterface.h"
#include "musicunityquerymovierequest.h"

/*! @brief The class of the wangyi query mv download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWYQueryMovieRequest : public MusicUnityQueryMovieRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicWYQueryMovieRequest(QObject *parent = nullptr);

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
    virtual void downLoadFinished() override final;

private Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    void downLoadSingleFinished();

private:
    /*!
     * Start to search mv list by id.
     */
    void parseFromMovieList(qint64 id);
    /*!
     * Start to search video list by id.
     */
    void parseFromVideoList(const QString &id);

};


/*! @brief The class of the wangyi query artist mv download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWYQueryArtistMovieRequest : public MusicQueryMovieRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicWYQueryArtistMovieRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by offset page.
     */
    virtual void startToPage(int offset) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

private:
    /*!
     * Get artist mv count.
     */
    void parseFromArtistMoviesCount(qint64 id);

};

#endif // MUSICWYQUERYMOVIEREQUEST_H
