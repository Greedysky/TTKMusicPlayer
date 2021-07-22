#include "fc14helper.h"
#include "decoder_fc14.h"

DecoderFC14::DecoderFC14(const QString &path)
    : Decoder()
{
    m_helper = new FC14Helper(path);
}

DecoderFC14::~DecoderFC14()
{
    delete m_helper;
}

bool DecoderFC14::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderFC14: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderFC14: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderFC14: initialize succes");
    return true;
}

qint64 DecoderFC14::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderFC14::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderFC14::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderFC14::seek(qint64 pos)
{
    m_helper->seek(pos);
}
