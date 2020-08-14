#include "sc68helper.h"
#include "decoder_sc68.h"

DecoderSC68::DecoderSC68(const QString &path)
    : Decoder()
{
    m_sc68 = new SC68Helper(path);
}

DecoderSC68::~DecoderSC68()
{
    delete m_sc68;
}

bool DecoderSC68::initialize()
{
    if(!m_sc68->initialize())
    {
        return false;
    }

    int rate = m_sc68->samplerate();
    int channels = m_sc68->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderSC68::totalTime() const
{
    return m_sc68->totalTime();
}

int DecoderSC68::bitrate() const
{
    return m_sc68->bitrate();
}

qint64 DecoderSC68::read(unsigned char *data, qint64 size)
{
    return m_sc68->read(data, size);
}

void DecoderSC68::seek(qint64 pos)
{
    m_sc68->seek(pos);
}
