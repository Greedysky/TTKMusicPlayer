#ifndef MUSICWYQUERYMOVIEREQUEST_H
#define MUSICWYQUERYMOVIEREQUEST_H

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

#include "musicwyqueryinterface.h"
#include "musicquerymovierequest.h"

/*! @brief The class of the wangyi mv query download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicWYQueryMovieRequest : public MusicQueryMovieRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicWYQueryMovieRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicWYQueryMovieRequest(QObject *parent = nullptr);

    /*!
     * Start to search data from name and type bt page.
     */
    virtual void startToPage(int offset) override final;
    /*!
     * Start to search data by type and input data.
     */
    virtual void startToSearch(QueryType type, const QString &value) override final;
    /*!
     * Start to search data by given id.
     */
    virtual void startToSingleSearch(const QString &id) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;
    /*!
     * Download page data from net finished.
     */
    void downLoadPageFinished();
    /*!
     * Download single data from net finished.
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
    /*!
     * Start to get video url path.
     */
    void parseFromVideoUrlPath(QString &url, const QString &id, int bitrate) const;
    /*!
     * Get artist mv count.
     */
    void parseFromArtistMoviesCount(qint64 id);

};

#endif // MUSICWYQUERYMOVIEREQUEST_H
