#include <string.h>
#include "visualbuffer_p.h"

static inline void stereo_from_multichannel(float *l, float *r, float *s, long cnt, int chan)
{
    if(chan == 1)
    {
        memcpy(l, s, cnt * sizeof(float));
        memcpy(r, s, cnt * sizeof(float));
        return;
    }

    while(cnt > 0)
    {
        l[0] = s[0];
        r[0] = s[1];
        s += chan;
        l++;
        r++;
        cnt--;
    }
}

void VisualBuffer::add(float *pcm, int samples, int channels, qint64 ts, qint64 delay)
{
    m_add_index++;
    m_add_index %= VISUAL_BUFFER_SIZE;
    VisualNode *b = &m_buffer[m_add_index];
    stereo_from_multichannel(b->data[0], b->data[1], pcm, qMin(512, samples / channels), channels);
    b->delta = m_time.elapsed();
    if(delay <= 0) //try to guess delay by elapsed time between function calls
    {
        for(int i = 0; i < VISUAL_BUFFER_SIZE; ++i)
            delay = qMax(m_buffer[i].delta, delay);
    }
    b->ts = ts + qBound(50LL, delay, 1000LL); //limit visualization delay
    m_elapsed = ts;
    m_time.restart();
}

VisualNode *VisualBuffer::take()
{
    int steps = 0;
    qint64 t = m_elapsed + m_time.elapsed();
    while((m_buffer[m_take_index].ts < t) && (steps++ < VISUAL_BUFFER_SIZE))
    {
        m_take_index++;
        m_take_index %= VISUAL_BUFFER_SIZE;
    }

    if(m_buffer[m_take_index].ts < t) //unable to find node
        return nullptr;

    if(m_buffer[m_take_index].ts > t + 100) //node is more than 100 ms in the future. So, ignore it.
        return nullptr;

    return &m_buffer[m_take_index];
}

void VisualBuffer::clear()
{
    m_take_index = 0;
    m_add_index = 0;
    m_elapsed = 0;
    for(int i = 0; i < VISUAL_BUFFER_SIZE; ++i)
    {
        m_buffer[i].ts = 0;
        m_buffer[i].delta = 0;
        memset(m_buffer[i].data[0], 0, 512 * sizeof(float));
        memset(m_buffer[i].data[1], 0, 512 * sizeof(float));
    }
}

QMutex *VisualBuffer::mutex()
{
    return &m_mutex;
}
