#include "outputportaudio.h"

#include <QSettings>

OutputPortAudio::OutputPortAudio()
    : Output()
{

}

OutputPortAudio::~OutputPortAudio()
{
    if(m_stream)
    {
        Pa_AbortStream(m_stream);
        Pa_CloseStream(m_stream);
        m_stream = nullptr;
    }

    if(m_initialized)
    {
        Pa_Terminate();
    }
}

bool OutputPortAudio::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    PaError err = Pa_Initialize();
    if(err != paNoError)
    {
        qWarning("OutputPortAudio: Pa_Initialize failed, message is %s", Pa_GetErrorText(err));
        return false;
    }

    m_initialized = true;
    const PaHostApiInfo *hostApiInfo = Pa_GetHostApiInfo(Pa_GetDefaultHostApi());
    if(!hostApiInfo)
    {
        qWarning("OutputPortAudio: Host API info not found");
        return false;
    }
    qDebug("OutputPortAudio: Using host API: %s", hostApiInfo->name);

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const QString &device_name = settings.value("PORTAUDIO/device", "default").toString();

    // Bufer
    // Using paFramesPerBufferUnspecified with alsa gives warnings about underrun
    const int buffer = settings.value("PORTAUDIO/buffer", paFramesPerBufferUnspecified).toInt();

    for(int index = 0; index < Pa_GetDeviceCount(); ++index)
    {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(index);
        if(deviceInfo && deviceInfo->name == device_name)
        {
            m_device = index;
            qDebug("OutputPortAudio: Device name is %s, buffer size is %d", deviceInfo->name, buffer);
            break;
        }
    }

    if(m_device == paNoDevice)
    {
        m_device = Pa_GetDefaultOutputDevice(); // choose default one
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(m_device);
        if(deviceInfo)
        {
            qDebug("OutputPortAudio: Use default device name is %s, buffer size is %d", deviceInfo->name, buffer);
        }

        if(!deviceInfo || m_device == paNoDevice)
        {
            qWarning("OutputPortAudio: Current device is invalid");
            return false;
        }
    }

    PaStreamParameters params;
    params.device = m_device;
    params.suggestedLatency = Pa_GetDeviceInfo(params.device)->defaultLowOutputLatency;
    params.channelCount = map.size();
    params.hostApiSpecificStreamInfo = nullptr;

//    err = Pa_IsFormatSupported(nullptr, &params, freq);
//    if(err != paNoError)
//    {
//        qWarning("OutputPortAudio: Unsupported format, message is %s", Pa_GetErrorText(err));
//        return false;
//    }

    switch(format)
    {
    case Qmmp::PCM_S8:
        params.sampleFormat = paInt8;
        break;
    case Qmmp::PCM_U8:
        params.sampleFormat = paUInt8;
        break;
    case Qmmp::PCM_S16LE:
        params.sampleFormat = paInt16;
        break;
    case Qmmp::PCM_S24LE:
        params.sampleFormat = paInt24;
        break;
    case Qmmp::PCM_S32LE:
        params.sampleFormat = paInt32;
        break;
    case Qmmp::PCM_FLOAT:
        params.sampleFormat = paFloat32;
        break;
    default:
        break;
    }

    err = Pa_OpenStream(&m_stream, nullptr, &params, freq, buffer, paNoFlag, nullptr, nullptr);
    if(err != paNoError || !m_stream)
    {
        qWarning("OutputPortAudio: Pa_OpenStream failed, message is %s", Pa_GetErrorText(err));
        return false;
    }

    err = Pa_StartStream(m_stream);
    if(err != paNoError)
    {
        qWarning("OutputPortAudio: Pa_StartStream failed, message is %s", Pa_GetErrorText(err));
        return false;
    }

    configure(freq, map, format);
    return true;
}

qint64 OutputPortAudio::latency()
{
    const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(m_device);
    return deviceInfo ? deviceInfo->defaultLowOutputLatency : 0;
}

qint64 OutputPortAudio::writeAudio(unsigned char *data, qint64 maxSize)
{
    if(!m_stream)
    {
        return -1;
    }

    Pa_WriteStream(m_stream, data, maxSize / (sampleSize() * channels()));
    return maxSize;
}

void OutputPortAudio::drain()
{
    Pa_AbortStream(m_stream);
}

void OutputPortAudio::reset()
{

}

void OutputPortAudio::suspend()
{
    Pa_StopStream(m_stream);
}

void OutputPortAudio::resume()
{
    Pa_StartStream(m_stream);
}
