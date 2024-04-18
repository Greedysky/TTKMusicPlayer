/***************************************************************************
 *   Copyright (C) 2008-2024 by Ilya Kotov                                 *
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
 
#ifndef VOLUMEHANDLER_H
#define VOLUMEHANDLER_H

#include <QMutex>
#include <atomic>
#include "volume.h"
#include "buffer.h"

class QTimer;
class SoftwareVolume;

/*!
 * @brief The VolumeHandler class provides volume control access
 * @author Ilya Kotov <forkotov02@ya.ru>
 */
class QMMP_EXPORT VolumeHandler : public QObject
{
    Q_OBJECT
public:
    /*!
     * Object constructor.
     * @param parent Parent object.
     */
    explicit VolumeHandler(QObject *parent = nullptr);
    /*!
     * Destructor.
     */
    ~VolumeHandler();
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
     * @param volume volume of the left and right channels \b [0..100].
     */
    void setVolume(int volume);
    /*!
     * Sets the balance between left and right channels.
     * @param balance balance between left and right channels \b [-100..100].
     */
    void setBalance(int balance);
    /*!
     * Sets volume mute state to \b muted.
     */
    void setMuted(bool muted);
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
    /*!
     * Returns \b true if volume is muted, otherwise returns \b false.
     */
    bool isMuted() const;
    /*!
     * Changes buffer volume (software mode only).
     * \param b \b Buffer pointer.
     * \param chan \b Number of channels.
     */
    void apply(Buffer *b, int chan);
    /*!
     * Returns a pointer to the global VolumeHandler instance.
     */
    static VolumeHandler *instance();

signals:
    /*!
     * Emitted when volume is changed.
     * @param left Left channel volume level. It should be \b 0..100
     * @param right Right channel volume level. It should be \b 0..100
     */
    void volumeChanged(int left, int right);
    /*!
     * Emitted when the highest volume of the left and right channels has changed.
     * @param volume New value of the highest volume of the left and right channels.
     */
    void volumeChanged(int volume);
    /*!
     * Emitted when the balance between left and right channels has changed.
     * @param balance New balance value.
     */
    void balanceChanged(int balance);
    /*!
     * Emitted when volume mute state has changed
     * \param muted New volume mute state.
     */
    void mutedChanged(bool muted);

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
    VolumeSettings m_settings;
    bool m_prev_block = false;
    std::atomic_bool m_muted = ATOMIC_VAR_INIT(false);
    std::atomic_bool m_apply = ATOMIC_VAR_INIT(false);
    QMutex m_mutex;
    double m_scaleLeft = 0, m_scaleRight = 0;
    Volume *m_volume = nullptr;
    QTimer *m_timer = nullptr;
    static VolumeHandler *m_instance;

};

#endif
