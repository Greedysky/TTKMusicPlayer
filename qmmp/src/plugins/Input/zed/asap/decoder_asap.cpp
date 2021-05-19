#include "asaphelper.h"
#include "decoder_asap.h"

DecoderAsap::DecoderAsap(const QString &path)
    : Decoder()
{
    m_helper = new AsapHelper(path);
}

DecoderAsap::~DecoderAsap()
{
    delete m_helper;
}

bool DecoderAsap::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderAsap: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderAsap: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderAsap: initialize succes");
    return true;
}

qint64 DecoderAsap::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderAsap::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderAsap::read(unsigned char *data, qint64 size)
{
    return m_helper->read(data, size);
}

void DecoderAsap::seek(qint64 pos)
{
    m_helper->seek(pos);
}
