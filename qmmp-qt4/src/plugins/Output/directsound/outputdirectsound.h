/***************************************************************************
 *   Copyright (C) 2014 by Ilya Kotov                                      *
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

#ifndef OUTPUTDIRECTSOUND_H
#define OUTPUTDIRECTSOUND_H

#include <stdio.h>
#include <windows.h>
#include <initguid.h>
#include <dsound.h>
#include <qmmp/volume.h>
#include <qmmp/output.h>

class VolumeDirectSound;

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class OutputDirectSound : public Output
{
public:
    OutputDirectSound();
    ~OutputDirectSound();

    bool initialize(quint32, ChannelMap map,  Qmmp::AudioFormat format);

    //output api
    qint64 latency();
    qint64 writeAudio(unsigned char *data, qint64 size);
    void drain();
    void suspend();
    void resume();
    void reset();

    //volume control
    static OutputDirectSound *instance;
    static VolumeDirectSound *volumeControl;
    IDirectSoundBuffer8 *secondaryBuffer();


private:
    // helper functions
    void status();
    void uninitialize();
    DWORD bytesToWrite();

    IDirectSound8 *m_ds;
    IDirectSoundBuffer *m_primaryBuffer;
    IDirectSoundBuffer8 *m_dsBuffer;
    DWORD m_dsBufferAt;
};

/**
    @author Ilya Kotov <forkotov02@hotmail.ru>
*/
class VolumeDirectSound : public Volume
{
public:
    VolumeDirectSound();
    ~VolumeDirectSound();

    void setVolume(const VolumeSettings &vol);
    VolumeSettings volume() const;
    void restore();

private:
    VolumeSettings m_volume;
};


#endif // OUTPUTDIRECTSOUND_H
