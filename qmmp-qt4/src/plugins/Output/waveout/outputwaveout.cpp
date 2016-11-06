/***************************************************************************
 *   Copyright (C) 2009-2013 by Ilya Kotov                                 *
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
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <qmmp/buffer.h>
#include <qmmp/visual.h>
#include "outputwaveout.h"

#define MAX_WAVEBLOCKS    256

static CRITICAL_SECTION  cs;
static HWAVEOUT          dev                    = NULL;
static unsigned int      ScheduledBlocks        = 0;
static int               PlayedWaveHeadersCount = 0;          // free index
static WAVEHDR*          PlayedWaveHeaders [MAX_WAVEBLOCKS];



static void CALLBACK wave_callback (HWAVE hWave, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    Q_UNUSED(hWave);
    Q_UNUSED(dwInstance);
    Q_UNUSED(dwParam2);
    if (uMsg == WOM_DONE)
    {
        EnterCriticalSection (&cs);
        PlayedWaveHeaders [PlayedWaveHeadersCount++] = (WAVEHDR*) dwParam1;
        ScheduledBlocks--;                        // decrease the number of USED blocks
        LeaveCriticalSection (&cs);
    }
}

static void
free_memory (void)
{
    WAVEHDR*  wh;
    HGLOBAL   hg;

    EnterCriticalSection (&cs);
    wh = PlayedWaveHeaders [--PlayedWaveHeadersCount];
    LeaveCriticalSection (&cs);

    waveOutUnprepareHeader (dev, wh, sizeof (WAVEHDR));

    hg = GlobalHandle (wh -> lpData);       // Deallocate the buffer memory
    GlobalUnlock (hg);
    GlobalFree   (hg);

    hg = GlobalHandle (wh);                 // Deallocate the header memory
    GlobalUnlock (hg);
    GlobalFree   (hg);
}

OutputWaveOut::OutputWaveOut() : Output()
{
    //m_connection = 0;
    //m_dev = 0;
}

OutputWaveOut::~OutputWaveOut()
{
    uninitialize();
}

bool OutputWaveOut::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    Q_UNUSED(format);
    if (!waveOutGetNumDevs ())
    {
        qWarning("OutputWaveOut: no audio device found");
        return false;
    }
    WAVEFORMATEX fmt;
    UINT deviceID = WAVE_MAPPER;

    fmt.wFormatTag = WAVE_FORMAT_PCM;
    fmt.wBitsPerSample  = 16;
    fmt.nChannels       = map.count();
    fmt.nSamplesPerSec  = (unsigned long)(freq);
    fmt.nBlockAlign     = fmt.nChannels * fmt.wBitsPerSample/8;
    fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nChannels * fmt.wBitsPerSample/8;

    switch (waveOutOpen (&dev, deviceID, &fmt, (DWORD)wave_callback, 0, CALLBACK_FUNCTION))
    {
    case MMSYSERR_ALLOCATED:
        qWarning("OutputWaveOut: Device is already open.");
        return false;
    case MMSYSERR_BADDEVICEID:
        qWarning("OutputWaveOut: The specified device is out of range.");
        return false;
    case MMSYSERR_NODRIVER:
        qWarning("OutputWaveOut: There is no audio driver in this system.");
        return false;
    case MMSYSERR_NOMEM:
        qWarning("OutputWaveOut: Unable to allocate sound memory.");
        return false;
    case WAVERR_BADFORMAT:
        qWarning("OutputWaveOut: This audio format is not supported.");
        return false;
    case WAVERR_SYNC:
        qWarning("OutputWaveOut: The device is synchronous.");
        return false;
    default:
        qWarning("OutputWaveOut: Unknown media error.");
        return false;
    case MMSYSERR_NOERROR:
        break;
    }

    waveOutReset (dev);
    InitializeCriticalSection (&cs);
    configure(freq, map, Qmmp::PCM_S16LE);

    return true;
}


qint64 OutputWaveOut::latency()
{
    return 0;
}

qint64 OutputWaveOut::writeAudio(unsigned char *data, qint64 len)
{
    HGLOBAL    hg;
    HGLOBAL    hg2;
    LPWAVEHDR  wh;
    void*      allocptr;
    len = qMin(len, (qint64)1024);


    while (PlayedWaveHeadersCount > 0)                        // free used blocks ...
        free_memory ();

    if (ScheduledBlocks >= sizeof(PlayedWaveHeaders)/sizeof(*PlayedWaveHeaders)) // wait for a free block ...
    {
        usleep(50000);
        return 0;
    }

    if ((hg2 = GlobalAlloc (GMEM_MOVEABLE, len)) == NULL)   // allocate some memory for a copy of the buffer
    {
        qWarning("OutputWaveOut: GlobalAlloc failed");
        return 0;
    }

    allocptr = GlobalLock (hg2);
    CopyMemory (allocptr, data, len);                         // Here we can call any modification output functions we want....

    if ((hg = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (WAVEHDR))) == NULL) // now make a header and WRITE IT!
        return -1;

    wh                   = (wavehdr_tag*)GlobalLock (hg);
    wh->dwBufferLength   = len;
    wh->lpData           = (CHAR *)allocptr;

    if (waveOutPrepareHeader (dev, wh, sizeof (WAVEHDR)) != MMSYSERR_NOERROR)
    {
        GlobalUnlock (hg);
        GlobalFree   (hg);
        return -1;
    }

    if (waveOutWrite (dev, wh, sizeof (WAVEHDR)) != MMSYSERR_NOERROR)
    {
        GlobalUnlock (hg);
        GlobalFree   (hg);
        return -1;
    }

    EnterCriticalSection (&cs);
    ScheduledBlocks++;
    LeaveCriticalSection (&cs);

    return len;
}

void OutputWaveOut::drain()
{
    while (ScheduledBlocks > 0)
        Sleep(10);
    while (PlayedWaveHeadersCount > 0)                        // free used blocks ...
        free_memory();
}

void OutputWaveOut::suspend()
{
    waveOutPause(dev);
}

void OutputWaveOut::resume()
{
    waveOutRestart(dev);
}

void OutputWaveOut::reset()
{
   while (PlayedWaveHeadersCount > 0)                        // free used blocks ...
      free_memory ();
   waveOutReset (dev);
}

void OutputWaveOut::uninitialize()
{
    if (dev)
    {
        waveOutReset(dev); // reset the device
        while (ScheduledBlocks > 0);
            Sleep (10);

        while (PlayedWaveHeadersCount > 0)         // free used blocks ...
            free_memory ();

        waveOutReset (dev);      // reset the device
        waveOutClose (dev);      // close the device
        dev = 0;
    }

    DeleteCriticalSection (&cs);
    return;
}

/***** MIXER *****/
VolumeWaveOut::VolumeWaveOut()
{}

VolumeWaveOut::~VolumeWaveOut()
{}

void VolumeWaveOut::setVolume(const VolumeSettings &vol)
{
    DWORD data = (vol.right*0xFFFF/100 << 16) | vol.left*0xFFFF/100;
    waveOutSetVolume(0, data);
}

VolumeSettings VolumeWaveOut::volume() const
{
    VolumeSettings vol;
    DWORD data;
    waveOutGetVolume(0, (PDWORD)&data);
    vol.left = (long)LOWORD(data) * 100 / 0xFFFF + 1;
    vol.right = (long)HIWORD(data) * 100 / 0xFFFF + 1;
    return vol;
}

bool VolumeWaveOut::isSupported() const
{
    WAVEOUTCAPS waveCaps;
    if(waveOutGetDevCaps(0,(LPWAVEOUTCAPS)&waveCaps, sizeof(WAVEOUTCAPS)) != MMSYSERR_NOERROR)
        return false;
    return (waveCaps.dwSupport & WAVECAPS_VOLUME) != 0;
}
