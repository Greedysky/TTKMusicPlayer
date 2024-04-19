#include "recycler_p.h"
#include "qmmpsettings.h"
#include "audioparameters.h"
#include "buffer.h"

Recycler::~Recycler()
{
    for(unsigned int i = 0; i < m_buffer_count; ++i)
    {
        delete m_buffers[i];
        m_buffers[i] = nullptr;
    }
    if(m_buffer_count)
        delete[] m_buffers;
    m_blocked = nullptr;
}

void Recycler::configure(quint32 freq, int chan)
{
    size_t block_samples = chan * QMMP_BLOCK_FRAMES;
    unsigned int buffer_count = freq * QmmpSettings::instance()->bufferSize() / 1000 / QMMP_BLOCK_FRAMES;
    if(block_samples == m_block_samples && buffer_count == m_buffer_count)
        return;

    for(unsigned int i = 0; i < m_buffer_count; ++i)
    {
        delete m_buffers[i];
        m_buffers[i] = nullptr;
    }
    if(m_buffer_count)
        delete[] m_buffers;
    m_add_index = 0;
    m_done_index = 0;
    m_current_count = 0;
    m_blocked = nullptr;
    m_block_samples = block_samples;
    m_buffer_count = buffer_count;


    if(m_buffer_count < 4)
        m_buffer_count = 4;

    m_buffers = new Buffer*[m_buffer_count];

    for(unsigned int i = 0; i < m_buffer_count; ++i)
    {
        m_buffers[i] = new Buffer(m_block_samples);
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
    if(full())
        return nullptr;
    return m_buffers[m_add_index];
}

void Recycler::add()
{
    if(m_buffers[m_add_index]->samples)
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
    return nullptr;
}

void Recycler::done()
{
    m_blocked = nullptr;
    if(m_current_count)
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

size_t Recycler::samples() const
{
    return m_buffer_count * m_block_samples;
}

size_t Recycler::blockSamples() const
{
    return m_block_samples;
}
