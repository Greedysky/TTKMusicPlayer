/***************************************************************************
 * This file is part of the TTK qmmp plugin project
 * Copyright (C) 2015 - 2026 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ***************************************************************************/

#ifndef OUTPUTPORTAUDIO_H
#define OUTPUTPORTAUDIO_H

extern "C"{
#include <portaudio.h>
}

#include <qmmp/output.h>
#include <qmmp/volume.h>

/**
    @author Greedysky <greedysky@163.com>
*/
class OutputPortAudio : public Output
{
public:
    OutputPortAudio();
    virtual ~OutputPortAudio();

    virtual bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format) override final;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 maxSize) override final;
    virtual void drain() override final;
    virtual void reset() override final;
    virtual void suspend() override final;
    virtual void resume() override final;

private:
    bool m_initialized = false;
    PaStream* m_stream = nullptr;
    PaDeviceIndex m_device = paNoDevice;

};

#endif
