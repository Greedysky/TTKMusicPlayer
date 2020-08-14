#include "v2mhelper.h"
#include "decoder_v2m.h"

DecoderV2M::DecoderV2M(const QString &path)
    : Decoder()
{
    m_v2m = new V2MHelper(path);
}

DecoderV2M::~DecoderV2M()
{
    delete m_v2m;
}

bool DecoderV2M::initialize()
{
    if(!m_v2m->initialize())
    {
        return false;
    }

    int rate = m_v2m->samplerate();
    int channels = m_v2m->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderV2M::totalTime() const
{
    return m_v2m->totalTime();
}

int DecoderV2M::bitrate() const
{
    return m_v2m->bitrate();
}

qint64 DecoderV2M::read(unsigned char *data, qint64 size)
{
    return m_v2m->read(data, size);
}

void DecoderV2M::seek(qint64 pos)
{
    m_v2m->seek(pos);
}
