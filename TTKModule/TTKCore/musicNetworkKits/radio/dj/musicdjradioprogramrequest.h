#ifndef MUSICDJRADIOPROGRAMREQUEST_H
#define MUSICDJRADIOPROGRAMREQUEST_H

/***************************************************************************
 * This file is part of the TTK Music Player project
 * Copyright (C) 2015 - 2025 Greedysky Studio

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

#include "musicabstractdjradiorequest.h"

/*! @brief The namespace of the program type.
 * @author Greedysky <greedysky@163.com>
 */
namespace TTK
{
    enum class Program
    {
        Recommed,
        Rank
    };
}

/*! @brief The class of the music dj radio program request.
 * @author Greedysky <greedysky@163.com>
 */
class TTK_MODULE_EXPORT MusicDJRadioProgramRequest : public MusicAbstractDJRadioRequest
{
    Q_OBJECT
    TTK_DECLARE_MODULE(MusicDJRadioProgramRequest)
public:
    /*!
     * Object constructor.
     */
    explicit MusicDJRadioProgramRequest(QObject *parent = nullptr);

    /*!
     * Start to download program data.
     */
    void startToRequest(TTK::Program type);

public Q_SLOTS:
    /*!
     * Download data from net finished.
     */
    virtual void downLoadFinished() override final;

};

#endif // MUSICDJRADIOPROGRAMREQUEST_H
