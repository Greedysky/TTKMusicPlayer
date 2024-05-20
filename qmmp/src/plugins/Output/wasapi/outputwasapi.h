/***************************************************************************
 *   Copyright (C) 2016-2024 by Ilya Kotov                                 *
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

#ifndef OUTPUTWASAPI_H
#define OUTPUTWASAPI_H

#include <stdio.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <qmmp/volume.h>
#include <qmmp/output.h>

class VolumeWASAPI;

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class OutputWASAPI : public Output
{
public:
    OutputWASAPI();
    virtual ~OutputWASAPI();

    virtual bool initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format) override final;

    virtual qint64 latency() override final;
    virtual qint64 writeAudio(unsigned char *data, qint64 size) override final;
    virtual void drain() override final;
    virtual void suspend() override final;
    virtual void resume() override final;
    virtual void reset() override final;

    // volume control
    ISimpleAudioVolume *simpleAudioVolume();
    static OutputWASAPI *instance;
    static VolumeWASAPI *volumeControl;

private:
    // helper functions
    void status();
    void uninitialize();

    IMMDeviceEnumerator *m_pEnumerator = nullptr;
    IMMDevice *m_pDevice = nullptr;
    IAudioClient *m_pAudioClient = nullptr;
    IAudioRenderClient *m_pRenderClient = nullptr;
    ISimpleAudioVolume *m_pSimpleAudioVolume = nullptr;

    UINT32 m_bufferFrames = 0;
    int m_frameSize = 0;
    QString m_id;
    bool m_exclusive = false;
    qint64 m_bufferSize = 1000000L; //microseconds

    typedef struct
    {
        Qmmp::ChannelPosition pos;
        DWORD chan_mask;
    } DWASAPIChannels;

    static DWASAPIChannels m_wasapi_pos[10];

};

/**
    @author Ilya Kotov <forkotov02@ya.ru>
*/
class VolumeWASAPI : public Volume
{
public:
    VolumeWASAPI();
    virtual ~VolumeWASAPI();

    virtual void setVolume(const VolumeSettings &vol) override final;
    virtual VolumeSettings volume() const override final;

    void restore();

private:
    VolumeSettings m_volume;

};

#endif
