#include "decoder_kss.h"
#include "ksshelper.h"

DecoderKSS::DecoderKSS(const QString &path)
    : Decoder()
{
    m_helper = new KSSHelper(path);
}

DecoderKSS::~DecoderKSS()
{
    delete m_helper;
}

bool DecoderKSS::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderKSS: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderKSS: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderKSS: initialize success");
    return true;
}

qint64 DecoderKSS::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderKSS::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderKSS::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderKSS::seek(qint64 time)
{
    m_helper->seek(time);
}
