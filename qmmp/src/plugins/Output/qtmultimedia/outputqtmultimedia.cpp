#include "outputqtmultimedia.h"

#include <unistd.h>
#include <QSettings>
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#  include <QAudioOutput>
#  include <QAudioDeviceInfo>
#else
#  include <QAudioSink>
#  include <QMediaDevices>
#endif

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
    qformat.setSampleRate(freq);
    qformat.setChannelCount(map.size());

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    qformat.setCodec("audio/pcm");
    qformat.setSampleType(QAudioFormat::SignedInt);

    QAudioFormat::Endian byteOrder = QAudioFormat::LittleEndian;
    switch(format)
    {
    case Qmmp::PCM_S16LE:
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S32LE:
        byteOrder = QAudioFormat::LittleEndian;
        break;
    case Qmmp::PCM_S16BE:
    case Qmmp::PCM_S24BE:
    case Qmmp::PCM_S32BE:
        byteOrder = QAudioFormat::BigEndian;
        break;
    default:
        break;
    }
    qformat.setByteOrder(byteOrder);

    int sampleSize = 16;
    switch(format)
    {
    case Qmmp::PCM_U8:
    case Qmmp::PCM_S8:
        sampleSize = 8;
        break;
    case Qmmp::PCM_S16LE:
    case Qmmp::PCM_S16BE:
        sampleSize = 16;
        break;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S24BE:
        sampleSize = 24;
        break;
    case Qmmp::PCM_S32LE:
    case Qmmp::PCM_S32BE:
        sampleSize = 32;
        break;
    default:
        break;
    }
    qformat.setSampleSize(sampleSize);
#else
    switch(format)
    {
    case Qmmp::PCM_U8:
    case Qmmp::PCM_S8:
        qformat.setSampleFormat(QAudioFormat::UInt8);
        break;
    case Qmmp::PCM_S16LE:
    case Qmmp::PCM_S16BE:
        qformat.setSampleFormat(QAudioFormat::Int16);
        break;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S24BE:
    case Qmmp::PCM_S32LE:
    case Qmmp::PCM_S32BE:
        qformat.setSampleFormat(QAudioFormat::Int32);
        break;
    case Qmmp::PCM_FLOAT:
        qformat.setSampleFormat(QAudioFormat::Float);
        break;
    default:
        break;
    }
#endif

    if(!qformat.isValid())
        return false;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    const QString &saved_device_name = settings.value("QTMULTIMEDIA/device").toString();

    //Size of sample representation in input data. For 24-bit is 4, high byte is ignored.
    const qint64 bytes_per_sample = AudioParameters::sampleSize(format);

    m_bytes_per_second = bytes_per_sample * freq * qformat.channelCount();

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QAudioDeviceInfo device_info;
    if(!saved_device_name.isEmpty())
    {
        const QList<QAudioDeviceInfo> &devices = QAudioDeviceInfo::availableDevices(QAudio::AudioOutput);
        for(const QAudioDeviceInfo &info : devices)
        {
            if(info.deviceName() == saved_device_name)
#else
    QAudioDevice device_info;
    if(!saved_device_name.isEmpty())
    {
        const QList<QAudioDevice> &devices = QMediaDevices::audioOutputs();
        for(const QAudioDevice &info : devices)
        {
            if(info.id() == saved_device_name)
#endif
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        device_info = QAudioDeviceInfo::defaultOutputDevice();
#else
        device_info = QMediaDevices::defaultAudioOutput();
#endif
        if(!device_info.isFormatSupported(qformat))
            return false;
    }

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    m_output = new QAudioOutput(device_info, qformat);
    const QString &device_name = device_info.deviceName();
#else
    m_output = new QAudioSink(device_info, qformat);
    m_output->setBufferSize(4096);
    const QString &device_name = device_info.description();
#endif
    qDebug("OutputQtMultimedia: Using output device: %s", qPrintable(device_name));

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


OutputControl::OutputControl(QtAudioOutput *o)
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
