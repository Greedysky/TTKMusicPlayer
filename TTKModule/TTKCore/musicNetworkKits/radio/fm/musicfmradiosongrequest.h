#ifndef MUSICFMRADIOSONGREQUEST_H
#define MUSICFMRADIOSONGREQUEST_H

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

#include "musicwydownloadtextrequest.h"

/*! @brief The class of the wangyi fm radio request of song info.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicFMRadioSongRequest : public MusicAbstractNetwork
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     */
    explicit MusicFMRadioSongRequest(QObject *parent = nullptr);

    /*!
     * Start to download radio data.
     */
    void startToRequest();

    /*!
     * Get media song information.
     */
    inline const TTK::MusicSongInformation& item() const noexcept { return m_item; }

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downloadFinished() override final;

private:
    TTK::MusicSongInformation m_item;

};

#endif // MUSICFMRADIOSONGREQUEST_H
