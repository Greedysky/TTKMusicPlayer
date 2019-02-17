/***************************************************************************
 *   Copyright (C) 2008-2019 by Ilya Kotov                                 *
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

#ifndef VOLUMECONTROL_P_H
#define VOLUMECONTROL_P_H

#include <QObject>
#include "qmmp.h"
#include "volume.h"
#include "buffer.h"

class QTimer;
class SoftwareVolume;

/*! @internal
 * @brief The VolumeControl class provides volume control access
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT VolumeControl : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     * @param parent Parent object.
     */
    VolumeControl(QObject *parent = 0);
    /*!
     * Destructor.
     */
    ~VolumeControl();
    /*!
     * Setups volume level.
     * Subclass should reimplement this fucntion.
     * @param left Left channel volume level. It should be \b 0..100
     * @param right Right channel volume level. It should be \b 0..100
     */
    void setVolume(int left, int right);
    /*!
     * Changes volume by \b delta percent
     */
    void changeVolume(int delta);
    /*!
     * Sets the volume of the left and right channels with keeping of the balance.
     * @param volume volume of the left and right channels \b[0..100].
     */
    void setVolume(int volume);
    /*!
     * Sets the balance between left and right channels.
     * @param balance balance between left and right channels \b [-100..100].
     */
    void setBalance(int balance);
    /*!
     * Returns left channel volume.
     */
    int left() const;
    /*!
     * Returns right channel volume.
     */
    int right() const;
    /*!
     * Returns the maximum volume of the left and right channels.
     */
    int volume() const;
    /*!
     * Returns the balance between left and right channels.
     */
    int balance() const;

signals:
    /*!
     * Emitted when volume is changed.
     * @param left Left channel volume level. It should be \b 0..100
     * @param right Right channel volume level. It should be \b 0..100
     */
    void volumeChanged(int left, int right);
    /*!
     * Emitted when the highest volume of the left and right channels has changed.
     * @param volume new value of the highest volume of the left and right channels.
     */
    void volumeChanged(int volume);
    /*!
     * Emitted when the balance between left and right channels has changed.
     * @param volume new balance value.
     */
    void balanceChanged(int balance);

public slots:
    /*!
     * Forces the volumeChanged signal to emit.
     */
    void checkVolume();
    /*!
     * Updates volume configuration
     */
    void reload();

private:
    int m_left, m_right;
    bool m_prev_block;
    Volume *m_volume;
    QTimer *m_timer;

};
/*! @internal
 * @brief The SoftwareVolume class provides access to the software volume control.
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT SoftwareVolume : public Volume
{
public:
    SoftwareVolume();
    ~SoftwareVolume();

    void setVolume(const VolumeSettings &v);
    VolumeSettings volume() const;
    void changeVolume(Buffer *b, int chan);

    static SoftwareVolume *instance();

private:
    int m_left, m_right;
    double m_scaleLeft, m_scaleRight;
    static SoftwareVolume *m_instance;
};

#endif
