#include "decoder_ken.h"
#include "kenhelper.h"

DecoderKen::DecoderKen(const QString &path)
    : Decoder()
{
    m_helper = new KenHelper(path);
}

DecoderKen::~DecoderKen()
{
    delete m_helper;
}

bool DecoderKen::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderKen: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderKen: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderKen: initialize success");
    return true;
}

qint64 DecoderKen::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderKen::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderKen::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderKen::seek(qint64 time)
{
    m_helper->seek(time);
}
