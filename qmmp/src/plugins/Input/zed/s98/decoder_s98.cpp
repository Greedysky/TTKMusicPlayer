#include "s98helper.h"
#include "decoder_s98.h"

DecoderS98::DecoderS98(const QString &path)
    : Decoder()
{
    m_helper = new S98Helper(path);
}

DecoderS98::~DecoderS98()
{
    delete m_helper;
}

bool DecoderS98::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderS98: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderS98: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderS98: initialize succes");
    return true;
}

qint64 DecoderS98::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderS98::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderS98::read(unsigned char *data, qint64 size)
{
    return m_helper->read(data, size);
}

void DecoderS98::seek(qint64 pos)
{
    m_helper->seek(pos);
}
