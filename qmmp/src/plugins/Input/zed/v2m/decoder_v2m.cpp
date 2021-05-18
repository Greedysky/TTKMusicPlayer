#include "v2mhelper.h"
#include "decoder_v2m.h"

DecoderV2M::DecoderV2M(const QString &path)
    : Decoder()
{
    m_helper = new V2MHelper(path);
}

DecoderV2M::~DecoderV2M()
{
    delete m_helper;
}

bool DecoderV2M::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderV2M: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderV2M: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_FLOAT);
    qDebug("DecoderV2M: initialize succes");
    return true;
}

qint64 DecoderV2M::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderV2M::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderV2M::read(unsigned char *data, qint64 size)
{
    return m_helper->read(data, size);
}

void DecoderV2M::seek(qint64 pos)
{
    m_helper->seek(pos);
}
