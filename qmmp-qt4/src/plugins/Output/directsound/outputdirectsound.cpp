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

#include <QObject>
#include <QSettings>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <qmmp/buffer.h>
#include <math.h>
#include "outputdirectsound.h"

#define DS_BUFSIZE (48*1024)

OutputDirectSound *OutputDirectSound::instance = 0;
VolumeDirectSound *OutputDirectSound::volumeControl = 0;

OutputDirectSound::OutputDirectSound() : Output()
{
    m_ds = 0;
    m_primaryBuffer = 0;
    m_dsBuffer = 0;
    m_dsBufferAt = 0;
    instance = this;
}

OutputDirectSound::~OutputDirectSound()
{
    instance = 0;
    uninitialize();
}

bool OutputDirectSound::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    Q_UNUSED(format);
    DSBUFFERDESC bufferDesc;

    HRESULT result = DirectSoundCreate8(0, &m_ds, 0);
    if(result != DS_OK)
    {
        qWarning("OutputDirectSound: DirectSoundCreate8 failed, error code = 0x%lx", result);
        m_ds = 0;
        return false;
    }

    if((result = m_ds->SetCooperativeLevel(GetDesktopWindow(), DSSCL_PRIORITY)) != DS_OK)
    {
        qWarning("OutputDirectSound: SetCooperativeLevel failed, error code = 0x%lx", result);
        return false;
    }

    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize        = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags       = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.lpwfxFormat   = NULL;

    if((result = m_ds->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL)) != DS_OK)
    {
        m_primaryBuffer = 0;
        qWarning("OutputDirectSound: CreateSoundBuffer failed, error code = 0x%lx", result);
        return false;
    }

    WAVEFORMATEX wfex;
    ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
    wfex.wFormatTag      = WAVE_FORMAT_PCM;
    wfex.nChannels       = map.count();
    wfex.nSamplesPerSec  = freq;
    wfex.wBitsPerSample  = 16;
    wfex.nBlockAlign     = (wfex.wBitsPerSample / 8) * wfex.nChannels;
    wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

    if((result = m_primaryBuffer->SetFormat(&wfex)) != DS_OK)
    {
        qWarning("OutputDirectSound: SetFormat failed, error code = 0x%lx", result);
        return false;
    }

    if((result = m_primaryBuffer->Play(0, 0, DSBPLAY_LOOPING)) != DS_OK)
    {
        qWarning("OutputDirectSound: Play failed, error code = 0x%lx", result);
        return false;
    }

    ZeroMemory(&wfex, sizeof(WAVEFORMATEX));
    wfex.wFormatTag      = WAVE_FORMAT_PCM;
    wfex.nChannels       = map.count();
    wfex.nSamplesPerSec  = freq;
    wfex.wBitsPerSample  = 16;
    wfex.nBlockAlign     = (wfex.wBitsPerSample / 8) * wfex.nChannels;
    wfex.nAvgBytesPerSec = wfex.nSamplesPerSec * wfex.nBlockAlign;

    ZeroMemory(&bufferDesc, sizeof(DSBUFFERDESC));
    bufferDesc.dwSize        = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags       = DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME |
            DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_CTRLPOSITIONNOTIFY;
    bufferDesc.lpwfxFormat   = &wfex;
    bufferDesc.dwBufferBytes = DS_BUFSIZE; // buffer size

    IDirectSoundBuffer *pDSB;
    if((result = m_ds->CreateSoundBuffer(&bufferDesc, &pDSB, NULL)) != DS_OK)
    {
        qWarning("OutputDirectSound: CreateSoundBuffer failed, error code = 0x%lx", result);
        return false;
    }

    if((result = pDSB->QueryInterface(IID_IDirectSoundBuffer8, (void**)&m_dsBuffer)) != DS_OK)
    {
        m_dsBuffer = 0;
        qWarning("OutputDirectSound: QueryInterface failed, error code = 0x%lx", result);
        pDSB->Release();
        return false;
    }

    m_dsBuffer->SetCurrentPosition(0);
    m_dsBuffer->Play(0,0,DSBPLAY_LOOPING);
    m_dsBufferAt = 0;
    configure(freq, map, Qmmp::PCM_S16LE);
    if(volumeControl)
        volumeControl->restore();
    return true;
}


qint64 OutputDirectSound::latency()
{
    return 0;
}

qint64 OutputDirectSound::writeAudio(unsigned char *data, qint64 len)
{
    unsigned char *ptr = 0, *ptr2 = 0;
    DWORD size = 0, size2 = 0;

    DWORD available = bytesToWrite(); //available bytes
    if(available < 128)
    {
        usleep(5000);
        return 0;
    }
    DWORD lockSize = qMin((DWORD)len, available); //required size

    HRESULT result = m_dsBuffer->Lock(m_dsBufferAt, lockSize,
                                      (void**)&ptr, (DWORD*)&size,
                                      (void**)&ptr2, (DWORD*)&size2, 0);
    if(result == DSERR_BUFFERLOST)
    {
        m_dsBuffer->Restore();
        result = m_dsBuffer->Lock(m_dsBufferAt, lockSize,
                                  (void**)&ptr, (DWORD*)&size,
                                  (void**)&ptr2, (DWORD*)&size2, 0);
    }
    if(result != DS_OK)
    {
        qWarning("OutputDirectSound: unable to lock buffer, error = 0x%lx", result);
        return -1;
    }

    DWORD totalSize = size + size2; //total locked size
    memmove(ptr, data, size);
    if(size2 > 0)
        memmove(ptr2, data + size, size2);

    m_dsBuffer->Unlock((void*)ptr, size, (void*)ptr2, size2);

    m_dsBufferAt += totalSize;
    m_dsBufferAt %= DS_BUFSIZE;
    return totalSize;
}

void OutputDirectSound::drain()
{
    DWORD dsCurrentPlayCursor = 0;
    m_dsBuffer->GetCurrentPosition((DWORD*)&dsCurrentPlayCursor, 0);

    while(dsCurrentPlayCursor >= m_dsBufferAt)
    {
        m_dsBuffer->GetCurrentPosition((DWORD*)&dsCurrentPlayCursor, 0);
    }
    while (dsCurrentPlayCursor <= m_dsBufferAt)
    {
        m_dsBuffer->GetCurrentPosition((DWORD*)&dsCurrentPlayCursor, 0);
    }
}

void OutputDirectSound::suspend()
{
    m_dsBuffer->Stop();
}

void OutputDirectSound::resume()
{
    HRESULT result = m_dsBuffer->Play(0,0,DSBPLAY_LOOPING);
    if(result == DSERR_BUFFERLOST)
    {
        result = m_dsBuffer->Play(0,0,DSBPLAY_LOOPING);
        m_dsBuffer->Restore();
    }
}

void OutputDirectSound::reset()
{
    m_dsBuffer->SetCurrentPosition(m_dsBufferAt-128);
}

IDirectSoundBuffer8 *OutputDirectSound::secondaryBuffer()
{
    return m_dsBuffer;
}

void OutputDirectSound::uninitialize()
{
    m_dsBufferAt = 0;
    if(m_dsBuffer)
    {
        m_dsBuffer->Stop();
        m_dsBuffer->Release();
        m_dsBuffer = 0;
    }
    if(m_primaryBuffer)
    {
        m_primaryBuffer->Stop();
        m_primaryBuffer->Release();
        m_primaryBuffer = 0;
    }
    if(m_ds)
    {
        m_ds->Release();
        m_ds = 0;
    }
}

DWORD OutputDirectSound::bytesToWrite()
{
    DWORD dsCurrentPlayCursor = 0;
    m_dsBuffer->GetCurrentPosition((DWORD*)&dsCurrentPlayCursor, 0);
    long available = dsCurrentPlayCursor - m_dsBufferAt; //available bytes

    if(available < 0)
    {
        available += DS_BUFSIZE;
    }
    return available;
}

/***** MIXER *****/
VolumeDirectSound::VolumeDirectSound()
{
    OutputDirectSound::volumeControl = this;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_volume.left = settings.value("DirectSound/left_volume", 100).toInt();
    m_volume.right = settings.value("DirectSound/right_volume", 100).toInt();
}

VolumeDirectSound::~VolumeDirectSound()
{
    m_volume = volume();
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.setValue("DirectSound/left_volume", m_volume.left);
    settings.setValue("DirectSound/right_volume", m_volume.right);
    OutputDirectSound::volumeControl = 0;
}

void VolumeDirectSound::setVolume(const VolumeSettings &vol)
{
    if(OutputDirectSound::instance && OutputDirectSound::instance->secondaryBuffer())
    {
        int maxVol = qMax(vol.left, vol.right);
        double voldB = -100.0, pandB = 0;
        if(maxVol)
        {
            voldB = 20.0*log(maxVol/100.0)/log(10);
            int balance = (vol.right - vol.left)*100.0/maxVol;
            pandB = balance ? 20.0*log((100.0 - fabs(balance))/100.0)/log(10) : 0;
            if(balance > 0)
                pandB = -pandB;
        }
        OutputDirectSound::instance->secondaryBuffer()->SetVolume(voldB*100);
        OutputDirectSound::instance->secondaryBuffer()->SetPan(pandB*100);
    }
    m_volume = vol;
}

VolumeSettings VolumeDirectSound::volume() const
{
    VolumeSettings vol;
    if(OutputDirectSound::instance && OutputDirectSound::instance->secondaryBuffer())
    {
        long v = 0;
        double voldB = 0, pandB = 0;
        OutputDirectSound::instance->secondaryBuffer()->GetVolume(&v);
        voldB = v / 100.0;
        OutputDirectSound::instance->secondaryBuffer()->GetPan(&v);
        pandB = v / 100.0;
        int volume = 100*pow(10, voldB/20.0);
        int balance = 100 - 100*pow(10, abs(pandB)/20.0);
        if(pandB > 0)
            balance = -balance;
        vol.left = volume-qMax(balance,0)*volume/100.0;
        vol.right = volume+qMin(balance,0)*volume/100.0;
        return vol;
    }
    return m_volume;
}

void VolumeDirectSound::restore()
{
    setVolume(m_volume);
}
