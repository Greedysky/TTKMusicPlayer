#ifndef MUSICKWBACKGROUNDTHREAD_H
#define MUSICKWBACKGROUNDTHREAD_H

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

#include "musicdownloadbackgroundthread.h"

/*! @brief The class of kuwo download art background image.
 * @author Greedysky <greedysky@163.com>
 */
class MUSIC_NETWORK_EXPORT MusicKWBackgroundThread : public MusicDownloadBackgroundThread
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicKWBackgroundThread)
public:
    /*!
     * Object contsructor provide artist name and save local path.
     */
    MusicKWBackgroundThread(const QString &name, const QString &save, QObject *parent = nullptr);

    /*!
     * Start to download artist picture from net.
     */
    virtual void startToDownload() override;

public Q_SLOTS:
    /*!
     * Download data from kuwo net finished.
     */
    void downLoadDataFinished(const QByteArray &bytes);

};

#endif // MUSICKWBACKGROUNDTHREAD_H
