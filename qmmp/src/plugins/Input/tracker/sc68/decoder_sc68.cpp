#include "decoder_sc68.h"
#include "sc68helper.h"

DecoderSC68::DecoderSC68(const QString &path)
    : Decoder()
{
    m_helper = new SC68Helper(path);
}

DecoderSC68::~DecoderSC68()
{
    delete m_helper;
}

bool DecoderSC68::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderSC68: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderSC68: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderSC68: initialize success");
    return true;
}

qint64 DecoderSC68::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderSC68::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderSC68::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderSC68::seek(qint64 time)
{
    m_helper->seek(time);
}
