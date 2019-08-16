#ifndef MUSICDOWNLOADCOUNTERPVTHREAD_H
#define MUSICDOWNLOADCOUNTERPVTHREAD_H

/* =================================================
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2019 Greedysky Studio

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

#include "musicnetworkabstract.h"

const QString QURTY_URL  = "S1AwTklDeUNQdG95dkIrNTdzZ1lBR3hMSFFqeTlGTjJjTlo4M3hYaHAyMHV3dTBFZ0x2R1dJVXJvOFN4REphRTFBS3BFeThHUzU3QVRjMHErTEFyZ2Y3N1NPRW0zYXh6anV2VGtUYzNUb3p6NHY0eQ==";
const QString HOST_URL   = "WUwzSXNrdXhZS2owUEpzWTN3V3NOVUNPTkZNdHBMWDY=";
const QString REFER_URL  = "a0RUSDNPMnFabWQwOVhVa2JFSkVtM0JCazVKRTIrbVFJWEpKd2g0Y1NLbz0=";
const QString COOKIE_URL = "cWpxdUI0YmIyNDdOcWp6TWZvMy9hYkdNN25UYVdpdStSUEV6Q3ZpMzNkRjd4WjV0ZmxzYnYxejRiTVJlUG5Wdi9UazlweFJPZGJlbVVpdGltTndId3NxNFUrSHNUcUQ5OENMdENnaUFOcElvTkpVcFVsRTQvS2Z3MndLNTNBb3Q=";

/*! @brief The class of get counter pv of player.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadCounterPVThread : public MusicNetworkAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadCounterPVThread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadCounterPVThread(QObject *parent = nullptr);

    virtual ~MusicDownloadCounterPVThread();

    /*!
     * Start to download counter pv from net.
     */
    void startToDownload();

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished();

};

#endif // MUSICDOWNLOADCOUNTERPVTHREAD_H
