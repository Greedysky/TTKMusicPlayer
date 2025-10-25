#include <unistd.h>
#include <stdio.h>
#include <iostream>

#include "outputwaveout.h"

#define MAX_WAVEBLOCKS    256

static CRITICAL_SECTION  cs;
static HWAVEOUT          dev                    = nullptr;
static unsigned int      ScheduledBlocks        = 0;
static int               PlayedWaveHeadersCount = 0;          // free index
static WAVEHDR*          PlayedWaveHeaders [MAX_WAVEBLOCKS];

static void CALLBACK wave_callback(HWAVE hWave, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    Q_UNUSED(hWave);
    Q_UNUSED(dwInstance);
    Q_UNUSED(dwParam2);
    if(uMsg == WOM_DONE)
    {
        EnterCriticalSection (&cs);
        PlayedWaveHeaders [PlayedWaveHeadersCount++] = (WAVEHDR*) dwParam1;
        ScheduledBlocks--;                        // decrease the number of USED blocks
        LeaveCriticalSection (&cs);
    }
}

static void free_memory(void)
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

OutputWaveOut::OutputWaveOut()
    : Output()
{

}

OutputWaveOut::~OutputWaveOut()
{
    uninitialize();
}

bool OutputWaveOut::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    Q_UNUSED(format);
    m_totalWritten = 0;
    if(!waveOutGetNumDevs())
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

    switch(waveOutOpen (&dev, deviceID, &fmt, (DWORD_PTR)wave_callback, 0, CALLBACK_FUNCTION))
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
    m_frameSize = channels() * sampleSize();
    return true;
}


qint64 OutputWaveOut::latency()
{
    MMTIME mmtime;
    mmtime.wType = TIME_SAMPLES;

    if(waveOutGetPosition(dev, &mmtime, sizeof(MMTIME)) != MMSYSERR_NOERROR)
        return 0;

    return (m_totalWritten / m_frameSize - mmtime.u.sample) * 1000 / sampleRate();
}

qint64 OutputWaveOut::writeAudio(unsigned char *data, qint64 maxSize)
{
    HGLOBAL    hg;
    HGLOBAL    hg2;
    LPWAVEHDR  wh;
    void*      allocptr;
    maxSize = qMin(maxSize, (qint64)1024);

    while(PlayedWaveHeadersCount > 0)                        // free used blocks ...
        free_memory();

    if(ScheduledBlocks >= sizeof(PlayedWaveHeaders)/sizeof(*PlayedWaveHeaders)) // wait for a free block ...
    {
        usleep(50000);
        return 0;
    }

    if((hg2 = GlobalAlloc (GMEM_MOVEABLE, maxSize)) == nullptr)   // allocate some memory for a copy of the buffer
    {
        qWarning("OutputWaveOut: GlobalAlloc failed");
        return 0;
    }

    allocptr = GlobalLock (hg2);
    CopyMemory (allocptr, data, maxSize);                         // Here we can call any modification output functions we want....

    if((hg = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (WAVEHDR))) == nullptr) // now make a header and WRITE IT!
        return -1;

    wh                   = (wavehdr_tag*)GlobalLock (hg);
    wh->dwBufferLength   = maxSize;
    wh->lpData           = (CHAR *)allocptr;

    if(waveOutPrepareHeader (dev, wh, sizeof (WAVEHDR)) != MMSYSERR_NOERROR)
    {
        GlobalUnlock (hg);
        GlobalFree   (hg);
        return -1;
    }

    if(waveOutWrite (dev, wh, sizeof (WAVEHDR)) != MMSYSERR_NOERROR)
    {
        GlobalUnlock (hg);
        GlobalFree   (hg);
        return -1;
    }

    EnterCriticalSection (&cs);
    ScheduledBlocks++;
    LeaveCriticalSection (&cs);

    m_totalWritten += maxSize;
    return maxSize;
}

void OutputWaveOut::drain()
{
    while(ScheduledBlocks > 0)
        Sleep(10);
    while(PlayedWaveHeadersCount > 0)                        // free used blocks ...
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
   while(PlayedWaveHeadersCount > 0)                        // free used blocks ...
      free_memory();
   waveOutReset (dev);
   m_totalWritten = 0;
}

void OutputWaveOut::uninitialize()
{
    if(dev)
    {
        waveOutReset(dev); // reset the device
        while(ScheduledBlocks > 0)
            Sleep (10);

        while(PlayedWaveHeadersCount > 0)         // free used blocks ...
            free_memory();

        waveOutReset (dev);      // reset the device
        waveOutClose (dev);      // close the device
        dev = nullptr;
    }

    DeleteCriticalSection (&cs);
    m_totalWritten = 0;
    return;
}

/***** MIXER *****/
void VolumeWaveOut::setVolume(const VolumeSettings &vol)
{
    DWORD data = (vol.right*0xFFFF/100 << 16) | vol.left*0xFFFF/100;
    waveOutSetVolume(nullptr, data);
}

VolumeSettings VolumeWaveOut::volume() const
{
    VolumeSettings vol;
    DWORD data;
    waveOutGetVolume(nullptr, (PDWORD)&data);
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
