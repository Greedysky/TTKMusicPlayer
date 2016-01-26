// Copyright (c) 2000-2001 Brad Hughes <bhughes@trolltech.com>
//
// Use, modification and distribution is allowed without limitation,
// warranty, or liability of any kind.
//

#include "recycler_p.h"
#include "qmmpsettings.h"
#include "audioparameters.h"
#include "buffer.h"

Recycler::Recycler ()
{
    m_add_index = 0;
    m_done_index = 0;
    m_current_count = 0;
    m_buffer_count = 0;
    m_blocked = 0;
    m_block_size = 0;
    m_buffers = 0;
}

Recycler::~Recycler()
{
    for (unsigned int i = 0; i < m_buffer_count; i++)
    {
        delete m_buffers[i];
        m_buffers[i] = 0;
    }
    if(m_buffer_count)
        delete [] m_buffers;
    m_blocked = 0;
}

void Recycler::configure(quint32 freq, int chan, Qmmp::AudioFormat format)
{
    unsigned long block_size = AudioParameters::sampleSize(format) * chan * QMMP_BLOCK_FRAMES;
    unsigned int buffer_count = freq * QmmpSettings::instance()->bufferSize() / 1000 / QMMP_BLOCK_FRAMES;
    if(block_size == m_block_size && buffer_count == m_buffer_count)
        return;

    for (unsigned int i = 0; i < m_buffer_count; i++)
    {
        delete m_buffers[i];
        m_buffers[i] = 0;
    }
    if(m_buffer_count)
        delete [] m_buffers;
    m_add_index = 0;
    m_done_index = 0;
    m_current_count = 0;
    m_blocked = 0;
    m_block_size = block_size;
    m_buffer_count = buffer_count;


    if (m_buffer_count < 4)
        m_buffer_count = 4;

    m_buffers = new Buffer*[m_buffer_count];

    for (unsigned int i = 0; i < m_buffer_count; i++)
    {
        m_buffers[i] = new Buffer(m_block_size);
    }
}

bool Recycler::full() const
{
    return m_current_count == m_buffer_count;
}

bool Recycler::blocked()
{
    return m_buffers[m_add_index] == m_blocked;
}


bool Recycler::empty() const
{
    return m_current_count == 0;
}


int Recycler::available() const
{
    return m_buffer_count - m_current_count;
}

int Recycler::used() const
{
    return m_current_count;
}


Buffer *Recycler::get()
{
    if (full())
        return 0;
    return m_buffers[m_add_index];
}

void Recycler::add()
{
    if(m_buffers[m_add_index]->nbytes)
    {
        m_add_index = (m_add_index + 1) % m_buffer_count;
        m_current_count++;
    }
}

Buffer *Recycler::next()
{
    if(m_current_count)
    {
        m_blocked = m_buffers[m_done_index];
        return m_blocked;
    }
    return 0;
}

void Recycler::done()
{
    m_blocked = 0;
    if (m_current_count)
    {
        m_current_count--;
        m_done_index = (m_done_index + 1) % m_buffer_count;
    }
}

void Recycler::clear()
{
    m_current_count = 0;
    m_add_index = 0;
    m_done_index = 0;
}

unsigned long Recycler::size() const
{
    return m_buffer_count * m_block_size;
}

unsigned long Recycler::blockSize() const
{
    return m_block_size;
}
