#ifndef MUSICDOWNLOADQNCONFIGHREAD_H
#define MUSICDOWNLOADQNCONFIGHREAD_H

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

const QString CONFIG_QURTY_URL  = "MDN2ZTFCbndoYkFzK2pQN2wzNVVKYWpwVTd2bXdKWjk0MmhLSjRuUFpROHpzWUhLcENKK2doUmVoa2NqMWFSaGdiaUwyTG5mL2tmelJSakJnT2dmcWJQYjRPTVR0SWROMGZWMkRkaEJtbWhFQ1loRFFSd3VReXl6bXFZPQ==";

/*! @brief The class of get qiniu config.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicDownloadQNConfighread : public MusicNetworkAbstract
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadQNConfighread)
public:
    /*!
     * Object contsructor.
     */
    explicit MusicDownloadQNConfighread(QObject *parent = nullptr);

    virtual ~MusicDownloadQNConfighread();

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

#endif // MUSICDOWNLOADQNCONFIGHREAD_H
