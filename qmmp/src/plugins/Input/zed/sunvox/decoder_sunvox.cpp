#include "decoder_sunvox.h"
#include "sunvoxhelper.h"

DecoderSunVox::DecoderSunVox(const QString &path)
    : Decoder()
{
    m_helper = new SunVoxHelper(path);
}

DecoderSunVox::~DecoderSunVox()
{
    delete m_helper;
}

bool DecoderSunVox::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderSunVox: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderSunVox: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderSunVox: initialize success");
    return true;
}

qint64 DecoderSunVox::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderSunVox::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderSunVox::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderSunVox::seek(qint64 time)
{
    m_helper->seek(time);
}
