#include "fc14helper.h"
#include "decoder_fc14.h"

DecoderFC14::DecoderFC14(const QString &path)
    : Decoder()
{
    m_fc14 = new FC14Helper(path);
}

DecoderFC14::~DecoderFC14()
{
    delete m_fc14;
}

bool DecoderFC14::initialize()
{
    if(!m_fc14->initialize())
    {
        qWarning("DecoderFC14: initialize failed");
        return false;
    }

    int rate = m_fc14->sampleRate();
    int channels = m_fc14->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderFC14: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderFC14::totalTime() const
{
    return m_fc14->totalTime();
}

int DecoderFC14::bitrate() const
{
    return m_fc14->bitrate();
}

qint64 DecoderFC14::read(unsigned char *data, qint64 size)
{
    return m_fc14->read(data, size);
}

void DecoderFC14::seek(qint64 pos)
{
    m_fc14->seek(pos);
}
