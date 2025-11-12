#include "decoder_tfmx.h"
#include "tfmxhelper.h"

DecoderTFMX::DecoderTFMX(const QString &path)
    : Decoder()
{
    m_helper = new TFMXHelper(path);
}

DecoderTFMX::~DecoderTFMX()
{
    delete m_helper;
}

bool DecoderTFMX::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderTFMX: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderTFMX: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderTFMX: initialize success");
    return true;
}

qint64 DecoderTFMX::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderTFMX::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderTFMX::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderTFMX::seek(qint64 time)
{
    m_helper->seek(time);
}
