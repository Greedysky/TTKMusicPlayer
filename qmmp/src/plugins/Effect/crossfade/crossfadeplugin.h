/***************************************************************************
 *   Copyright (C) 2010-2024 by Ilya Kotov                                 *
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

#ifndef CROSSFADEPLUGIN_H
#define CROSSFADEPLUGIN_H

#include <stddef.h>
#include <qmmp/effect.h>

class SoundCore;
class StateHandler;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class CrossfadePlugin : public Effect
{
public:
    CrossfadePlugin();
    virtual ~CrossfadePlugin();

    virtual void applyEffect(Buffer *b) override final;

private:
    enum State
    {
        Waiting = 0,
        Checking,
        Preparing,
        Processing,
    };

    void mix(float *cur_buf, float *prev_buf, uint samples, double volume);

    float *m_buffer = nullptr;
    size_t m_bufferAt = 0;
    size_t m_bufferSize = 0;
    qint64 m_overlap;
    int m_state = Waiting;
    SoundCore *m_core;
    StateHandler *m_handler;

};

#endif
