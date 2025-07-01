#include "outputqtmultimedia.h"

#include <unistd.h>
#include <QSettings>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>

OutputQtMultimedia::OutputQtMultimedia()
    : Output()
{

}

OutputQtMultimedia::~OutputQtMultimedia()
{
    if(m_output && m_control)
        QMetaObject::invokeMethod(m_control, "stop", Qt::QueuedConnection);

    if(m_output)
        m_output->deleteLater();
    if(m_control)
        m_control->deleteLater();
}

bool OutputQtMultimedia::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    QAudioFormat qformat;
    qformat.setCodec("audio/pcm");
    qformat.setSampleRate(freq);
    qformat.setByteOrder(QAudioFormat::LittleEndian);
    qformat.setChannelCount(map.size());
    qformat.setSampleType(QAudioFormat::SignedInt);

    //Size of sample representation in input data. For 24-bit is 4, high byte is ignored.
    const qint64 bytes_per_sample = AudioParameters::sampleSize(format);

    switch(format)
    {
    case Qmmp::PCM_S8:
        qformat.setSampleSize(8);
        break;
    case Qmmp::PCM_S16LE:
        qformat.setSampleSize(16);
        break;
    case Qmmp::PCM_S24LE:
        qformat.setSampleSize(24);
        break;
    case Qmmp::PCM_S32LE:
        qformat.setSampleSize(32);
        break;
    default:
        break;
    }

    if(!qformat.isValid())
        return false;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const QString &saved_device_name = settings.value("QTMULTIMEDIA/device").toString();

    m_bytes_per_second = bytes_per_sample * freq * qformat.channelCount();

    QAudioDeviceInfo device_info;
    if(!saved_device_name.isEmpty())
    {
        const QList<QAudioDeviceInfo> &devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        for(const QAudioDeviceInfo &info : devices)
        {
            if(info.deviceName() == saved_device_name)
            {
                if(info.isFormatSupported(qformat))
                {
                    device_info = info;
                    break;
                }
                else
                    qDebug("OutputQtMultimedia: Output device: %s is not supported", qPrintable(saved_device_name));
            }
        }
    }

    if(device_info.isNull())
    {
        device_info = QAudioDeviceInfo::defaultOutputDevice();
        if(!device_info.isFormatSupported(qformat))
            return false;
    }

    qDebug("OutputQtMultimedia: Using output device: %s", qPrintable(device_info.deviceName()));

    m_output = new QAudioOutput(device_info, qformat);
    m_buffer = m_output->start();
    m_control = new OutputControl(m_output);

    configure(freq, map, format);
    return true;
}

qint64 OutputQtMultimedia::latency()
{
    return 0;
}

qint64 OutputQtMultimedia::writeAudio(unsigned char *data, qint64 maxSize)
{
    if(!m_output->bytesFree()) {
        //If the buffer is full, waiting for some bytes to be played:
        //trying to play maxSize bytes, but not more than half of buffer.
        usleep(qMin(maxSize, static_cast<qint64>(m_output->bufferSize() / 2)) * 1000000 / m_bytes_per_second);
    }
    return m_buffer->write((const char*)data, maxSize);
}

void OutputQtMultimedia::drain()
{
    m_buffer->waitForBytesWritten(-1);
}

void OutputQtMultimedia::reset()
{
    m_buffer->reset();
}

void OutputQtMultimedia::suspend()
{
    QMetaObject::invokeMethod(m_control, "suspend", Qt::QueuedConnection);
}

void OutputQtMultimedia::resume()
{
    QMetaObject::invokeMethod(m_control, "resume", Qt::QueuedConnection);
}


OutputControl::OutputControl(QAudioOutput *o)
    : m_output(o)
{

}

void OutputControl::suspend()
{
    m_output->suspend();
}

void OutputControl::resume()
{
    m_output->resume();
}

void OutputControl::stop()
{
    m_output->stop();
}
