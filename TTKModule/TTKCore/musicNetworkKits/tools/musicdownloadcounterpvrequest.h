#ifndef MUSICDOWNLOADCOUNTERPVREQUEST_H
#define MUSICDOWNLOADCOUNTERPVREQUEST_H

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

#include "musicabstractnetwork.h"

const QString QUERY_URL  = "QkZ1ek5yL0gxbkV6eGU5bEttakZNT2pyNitIZVZoYlNBYlUxMEp2c2swQWdQODRNZmJmdWVqeGIrK0VsRzNwWUNDZjBkU3J6dmlwQklRRVZ6WVdsa2kwQmRhR0YrbmlZRERWSmlzazNNU21EbWRjS0sxSmtKNXZEekt5MWNwTmgvQ2hBOXo4MWlCajF3c2dXMTdTUXNIQmxCbzdCeC9HUC9MR09Sem80dVZRZmpVeDFkUXU1cVpPMUJIeXJIQTlJRHdTZjM2YWZ6ZWdTYTV1dExnUGd3OG9MekxCczJuS0RVbE1iK21DYkxENEhHQ0JwSHU1bVpuVklOY1Z6YU0walRldUxoL3pHOGpoWEVKOWNST0JZcEZoWWtLQT0=";

/*! @brief The class of the counter pv of player.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadCounterPVRequest : public MusicAbstractNetwork
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadCounterPVRequest)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadCounterPVRequest(QObject *parent = nullptr);

    /*!
     * Start to download data from net.
     */
    void startRequest();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

};

#endif // MUSICDOWNLOADCOUNTERPVREQUEST_H
