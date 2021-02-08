#include "asaphelper.h"
#include "decoder_asap.h"

DecoderAsap::DecoderAsap(const QString &path)
    : Decoder()
{
    m_asap = new AsapHelper(path);
}

DecoderAsap::~DecoderAsap()
{
    delete m_asap;
}

bool DecoderAsap::initialize()
{
    if(!m_asap->initialize())
    {
        qWarning("DecoderAsap: initialize failed");
        return false;
    }

    int rate = m_asap->sampleRate();
    int channels = m_asap->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderAsap: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderAsap::totalTime() const
{
    return m_asap->totalTime();
}

int DecoderAsap::bitrate() const
{
    return m_asap->bitrate();
}

qint64 DecoderAsap::read(unsigned char *data, qint64 size)
{
    return m_asap->read(data, size);
}

void DecoderAsap::seek(qint64 pos)
{
    m_asap->seek(pos);
}
