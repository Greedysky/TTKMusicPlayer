/***************************************************************************
 *   Copyright (C) 2017 by Ilya Kotov                                      *
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

#include <string.h>
#include "visualbuffer_p.h"

static inline void stereo_from_multichannel(float *l,
                                              float *r,
                                              float *s,
                                              long cnt, int chan)
{
    if(chan == 1)
    {
        memcpy(l, s, cnt * sizeof(float));
        memcpy(r, s, cnt * sizeof(float));
        return;
    }
    while (cnt > 0)
    {
        l[0] = s[0];
        r[0] = s[1];
        s += chan;
        l++;
        r++;
        cnt--;
    }
}

VisualBuffer::VisualBuffer()
{
    m_take_index = 0;
    m_add_index = 0;
    m_elapsed = 0;
}

void VisualBuffer::add(float *pcm, int samples, int channels, qint64 ts, qint64 delay)
{
    m_add_index++;
    m_add_index %= VISUAL_BUFFER_SIZE;
    VisualNode *b = &m_buffer[m_add_index];
    stereo_from_multichannel(b->data[0], b->data[1], pcm, qMin(512, samples / channels), channels);
    b->ts = ts;
    delay = qBound(50LL, delay, 1000LL); //limit visualization delay
    m_elapsed = qMax(0LL, ts - delay);
    m_time.restart();
}

VisualNode *VisualBuffer::take()
{
    int steps = 0;
    int t = m_elapsed + m_time.elapsed();
    while(m_buffer[m_take_index].used ||
          ((m_buffer[m_take_index].ts < t) && (steps++ < VISUAL_BUFFER_SIZE)))
    {
        m_take_index++;
        m_take_index %= VISUAL_BUFFER_SIZE;
    }
    if(m_buffer[m_take_index].ts < t) //unable to find node
        return 0;

    if(m_buffer[m_take_index].ts > t + 100) //node is more than 100 ms in the future. So, ignore it.
        return 0;

    return &m_buffer[m_take_index];
}

void VisualBuffer::clear()
{
    m_take_index = 0;
    m_add_index = 0;
    for(int i = 0; i < VISUAL_BUFFER_SIZE; ++i)
    {
        m_buffer[i].ts = 0;
        m_buffer[i].used = false;
        memset(m_buffer[i].data[0], 0, 512 * sizeof(float));
        memset(m_buffer[i].data[1], 0, 512 * sizeof(float));
    }
}

QMutex *VisualBuffer::mutex()
{
    return &m_mutex;
}
