/***************************************************************************
 *   Copyright (C) 2010-2025 by Ilya Kotov                                 *
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

#ifndef RECYCLER_P_H
#define RECYCLER_P_H

#include <QMutex>
#include <QWaitCondition>
#include "buffer.h"

/*! @internal
 *  @brief The Recycler class provides a queue of audio buffers.
 *  @author Brad Hughes <bhughes@trolltech.com>
 */
class QMMP_EXPORT Recycler
{
public:
    /*!
     * Constructor.
     */
    Recycler() = default;
    /*!
     * Destructor.
     */
    ~Recycler();

    /*!
     * Setups audio parameters of output interface.
     * @param freq Sample rate.
     * @param chan Number of channels.
     */
    void configure(quint32 freq, int chan);
    /*!
     * Returns \b true if queue if full, otherwise returns \b false
     */
    bool full() const;
    /*!
     * Returns \b true if queue if empty, otherwise returns \b false
     */
    bool empty() const;
    /*!
     * Returns a number of available buffers.
     */
    int available() const;
    /*!
     * Returns a number of used buffers.
     */
    int used() const;
    /*!
     *  Returns next buffer in queue.
     */
    Buffer *next();
    /*!
     * Returns current buffer for writing.
     */
    Buffer *get(); // get next in recycle
    /*!
     * Adds current buffer to queue.
     */
    void add(); // add to queue
    /*!
     * Removes current buffer from queue.
     */
    void done(); // add to recycle
    /*!
     * Clears queue.
     */
    void clear(); // clear queue
    /*!
     * Returns size of all buffers in samples.
     */
    size_t samples() const; // size in samples
    /*!
     * Returns block size in samples.
     */
    size_t blockSamples() const; // size in samples

    /*!
    * Returns mutex pointer.
    */
    QMutex *mutex()
    {
        return &m_mtx;
    }
    /*!
     * Returns wait condition pointer.
     */
    QWaitCondition *cond()
    {
        return &m_cnd;
    }
    /*!
     * Returns \b true if the next buffer is used by output. Otherwise returns \b false.
     */
    bool blocked();

private:
    unsigned int m_buffer_count = 0, m_add_index = 0, m_done_index = 0, m_current_count = 0;
    size_t m_block_samples = 0;
    Buffer **m_buffers = nullptr;
    QMutex m_mtx;
    QWaitCondition m_cnd;
    Buffer *m_blocked = nullptr;

};

#endif
