#ifndef MUSICDOWNLOADSTATUSMODULE_H
#define MUSICDOWNLOADSTATUSMODULE_H

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

#include "musicstringutils.h"

class MusicApplication;

/*! @brief The class of the show download state object.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDownloadStatusModule : public QObject
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDownloadStatusModule)
public:
    /*!
     * Object constructor.
     */
    explicit MusicDownloadStatusModule(QObject *parent = nullptr);

    /*!
     * Check the current song meta data valid.
     */
    void checkMetaDataValid(bool mode);

public Q_SLOTS:
    /*!
     * Download current music meta data from net.
     */
    void currentMetaDataDownload();
    /*!
     * Show download state string type.
     */
    void showDownloadInfoFinished(const QString &bytes);
    /*!
     * Show current net connect state changed by net request.
     */
    void networkConnectionStateChanged(bool state);

private:
    /*!
     * Check the current lrc meta data valid.
     */
    bool checkLrcValid() const;
    /*!
     * Check the current artist cover meta data valid.
     */
    bool checkArtistCoverValid() const;
    /*!
     * Check the current artist background meta data valid.
     */
    bool checkArtistBackgroundValid() const;

    bool m_previousState;
    MusicApplication *m_parent;

};

#endif // MUSICDOWNLOADSTATUSMODULE_H
