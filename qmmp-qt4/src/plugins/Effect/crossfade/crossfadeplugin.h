/***************************************************************************
 *   Copyright (C) 2010-2015 by Ilya Kotov                                 *
 *   forkotov02@hotmail.ru                                                 *
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
#ifndef CROSSFADEPLUGIN_H
#define CROSSFADEPLUGIN_H

#include <QMutex>
#include <stddef.h>
#include <qmmp/effect.h>

class SoundCore;
class StateHandler;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/

class CrossfadePlugin : public Effect
{
public:
    CrossfadePlugin();

    virtual ~CrossfadePlugin();

    void applyEffect(Buffer *b);
    void configure(quint32 freq, ChannelMap map);

private:
    enum State
    {
        WAITING = 0,
        CHECKING,
        PREPARING,
        PROCESSING,
    };

    void mix(float *cur_buf, float *prev_buf, uint samples, double volume);

    float *m_buffer;
    size_t m_buffer_at;
    size_t m_buffer_size;
    qint64 m_overlap;
    int m_state;
    SoundCore *m_core;
    StateHandler *m_handler;

};

#endif
