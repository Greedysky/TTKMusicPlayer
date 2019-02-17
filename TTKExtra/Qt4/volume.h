/***************************************************************************
 *   Copyright (C) 2012-2019 by Ilya Kotov                                 *
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

#ifndef VOLUME_H
#define VOLUME_H

#include <QObject>
#include "qmmp_export.h"

/*! @brief The VolumeSettings structure stores volume levels
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
struct QMMP_EXPORT VolumeSettings
{
    /*!
     * Constructor
     */
    VolumeSettings()
    {
        left = 0;
        right = 0;
    }
    int left /*!< Volume of the left channel. It should be \b 0..100. */;
    int right /*!< Volume of the left channel It should be \b 0..100. */;
};

/*! @brief The Volume class provides asbtract volume interface
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT Volume : public QObject
{
    Q_OBJECT
public:
    /*!
     * Destructor.
     */
    virtual ~Volume(){}
    /*!
     * Setups volume levels.
     * Subclass should reimplement this fucntion.
     * @param volume Structure with required volume levels.
     */
    virtual void setVolume(const VolumeSettings &volume) = 0;
    /*!
     * Returns volume level of the \b channel.
     */
    virtual VolumeSettings volume() const = 0;
    /*!
     * Returns true if the object supports change notification via
     * emitting changed() signal so polling the volume is not needed.
     */
    virtual bool hasNotifySignal() const;

signals:
    /*!
     * Emitted if volume is changed.
     */
    void changed();
};

#endif // VOLUME_H
