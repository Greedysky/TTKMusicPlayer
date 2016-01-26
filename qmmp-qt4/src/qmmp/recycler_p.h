// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//

#ifndef RECYCLER_P_H
#define RECYCLER_P_H

#include <QMutex>
#include <QWaitCondition>
#include "qmmp.h"
#include "buffer.h"

/*! @internal
 *  @brief The Recycler class provides a queue of audio buffers.
 *  @author Brad Hughes <bhughes@trolltech.com>
 */
class Recycler
{
public:
    /*!
     * Constructor.
     */
    Recycler();
    /*!
     * Destructor.
     */
    ~Recycler();
    /*!
     * Setups audio parameters of output interface.
     * @param freq Sample rate.
     * @param chan Number of channels.
     * @param format Audio format
     */
    void configure(quint32 freq, int chan, Qmmp::AudioFormat format);
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
     * Returns size of all buffers in bytes.
     */
    unsigned long size() const; // size in bytes
    /*!
     * Returns block size in bytes.
     */
    unsigned long blockSize() const; // size in bytes

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
    unsigned int m_buffer_count, m_add_index, m_done_index, m_current_count;
    unsigned long m_block_size;
    Buffer **m_buffers;
    QMutex m_mtx;
    QWaitCondition m_cnd;
    Buffer *m_blocked;
};

#endif // __recycler_h
