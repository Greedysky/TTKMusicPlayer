#ifndef MUSICKWQUERYARTISTREQUEST_H
#define MUSICKWQUERYARTISTREQUEST_H

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

#include "musickwqueryinterface.h"
#include "musicqueryartistrequest.h"

/*! @brief The class of the kuwo query artist download data from net.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicKWQueryArtistRequest : public MusicQueryArtistRequest
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicKWQueryArtistRequest(QObject *parent = nullptr);

    /*!
     * Start to search data by offset page.
     */
    virtual void startToPage(int offset) override final;
    /*!
     * Start to download query result data.
     */
    virtual void startToQueryResult(TTK::MusicSongInformation *info, int bitrate) override final;

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

private:
    /*!
     * Get Download introduction data from net.
     */
    void queryArtistIntro(MusicResultDataItem *item) const;

};

#endif // MUSICKWQUERYARTISTREQUEST_H
