/***************************************************************************
 *   Copyright (C) 2012-2026 by Ilya Kotov                                 *
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

#include <QFlags>
#include <QObject>
#include "qmmp_export.h"

/*! @brief The VolumeSettings structure stores volume levels
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
struct QMMP_EXPORT VolumeSettings
{
    int left = 0;  /*!< Volume of the left channel. It should be \b 0..100. */
    int right = 0; /*!< Volume of the left channel It should be \b 0..100. */

};

inline bool operator==(const VolumeSettings &v1, const VolumeSettings &v2)
{
    return v1.left == v2.left && v1.right == v2.right;
}

inline bool operator!=(const VolumeSettings &v1, const VolumeSettings &v2)
{
    return v1.left != v2.left || v1.right != v2.right;
}

/*! @brief The Volume class provides asbtract volume interface
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT Volume : public QObject
{
    Q_OBJECT
public:
    /*!
     * This enum describes volume capabilities.
     */
    enum VolumeFlag
    {
        IsMuteSupported = 0x1, /*!< Indicates the interface has feature to mute audio */
        HasNotifySignal = 0x2, /*!< Indicates the object supports change notification via
                                * emitting changed() signal so polling the volume is not needed */
    };
    Q_DECLARE_FLAGS(VolumeFlags, VolumeFlag)

    /*!
     * Destructor.
     */
    virtual ~Volume() = default;

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
     * Returns \b true if volume is disabled. Otherwise returns \b false.
     */
    virtual bool isMuted() const;
    /*!
     * Mutes/Restores volume. Default implementation does nothing.
     * @param mute - state of volume (\b true - mute, \b false - restore)
     */
    virtual void setMuted(bool mute);
    /*!
     * Returns volume flags.
     */
    virtual VolumeFlags flags() const;

signals:
    /*!
     * Emitted if volume is changed.
     */
    void changed();

private:
    bool m_mutedInternal = false;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Volume::VolumeFlags)

#endif
