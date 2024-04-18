/***************************************************************************
 *   Copyright (C) 2014-2024 by Ilya Kotov                                 *
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

#ifndef OUTPUTDIRECTSOUND_H
#define OUTPUTDIRECTSOUND_H

#include <stdio.h>
#include <windows.h>
#include <ksmedia.h>
#include <mmreg.h>
#include <dsound.h>
#include <qmmp/volume.h>
#include <qmmp/output.h>

class VolumeDirectSound;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OutputDirectSound : public Output
{
public:
    OutputDirectSound();
    virtual ~OutputDirectSound();

    virtual bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format) override final;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 size) override final;
    virtual void drain() override final;
    virtual void suspend() override final;
    virtual void resume() override final;
    virtual void reset() override final;

    // volume control
    static OutputDirectSound *instance;
    static VolumeDirectSound *volumeControl;
    IDirectSoundBuffer8 *secondaryBuffer();

private:
    // helper functions
    void status();
    void uninitialize();
    DWORD bytesToWrite();

    IDirectSound8 *m_ds = nullptr;
    IDirectSoundBuffer *m_primaryBuffer = nullptr;
    IDirectSoundBuffer8 *m_dsBuffer = nullptr;
    DWORD m_dsBufferAt = 0;

    typedef struct
    {
        Qmmp::ChannelPosition pos;
        DWORD chan_mask;
    } DSoundChannels;

    static DSoundChannels m_dsound_pos[10];
    qint64 m_latency = 0;
    qint32 m_bytesPerSecond = 0;
    bool m_reset = false;

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VolumeDirectSound : public Volume
{
public:
    VolumeDirectSound();
    virtual ~VolumeDirectSound();

    virtual void setVolume(const VolumeSettings &vol) override final;
    virtual VolumeSettings volume() const override final;

    void restore();

private:
    VolumeSettings m_volume;

};

#endif
