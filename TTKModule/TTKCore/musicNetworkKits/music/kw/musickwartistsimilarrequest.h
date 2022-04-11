#ifndef MUSICKWARTISTSIMILARREQUEST_H
#define MUSICKWARTISTSIMILARREQUEST_H

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

#include "musicsimilarrequest.h"

/*! @brief The class of query kuwo artist similar download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKWArtistSimilarRequest : public MusicSimilarRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicKWArtistSimilarRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicKWArtistSimilarRequest(QObject *parent = nullptr);

    /*!
     * Start to Search data from name.
     */
    virtual void startToSearch(const QString &value) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

protected:
    /*!
     * Get artist name by id.
     */
    QString artistNameById(const QString &id) const;
    /*!
     * Get id by artist name.
     */
    QString artistIdName(const QString &name) const;

};


#endif // MUSICKWARTISTSIMILARREQUEST_H
