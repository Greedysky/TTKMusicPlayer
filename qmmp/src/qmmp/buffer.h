/***************************************************************************
 *   Copyright (C) 2014-2025 by Ilya Kotov                                 *
 *   forkotov02@ya.ru                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <QSharedPointer>
#include "trackinfo.h"

#define QMMP_BLOCK_FRAMES 512

/*! @brief Audio buffer class.
 * @author Brad Hughes <bhughes@trolltech.com>
 */
class QMMP_EXPORT Buffer
{
public:
    /*!
     * Constructs an empty buffer object.
     * @param sz Size in samples;
     */
    explicit Buffer(size_t sz)
        : size(sz)
    {
        data = new float[sz];
    }
    /*!
     * Destructor.
     */
    ~Buffer()
    {
        delete[] data;
        data = nullptr;
        samples = 0;
        rate = 0;
        size = 0;
    }

    float *data = nullptr;               /*!< Audio data. */
    size_t samples = 0;                  /*!< Audio data size in samples. */
    size_t size = 0;                     /*!< Buffer size in samples. */
    unsigned int rate = 0;               /*!< Buffer bitrate. */
    QSharedPointer<TrackInfo> trackInfo; /*!< Attached track information. */

};

#endif
