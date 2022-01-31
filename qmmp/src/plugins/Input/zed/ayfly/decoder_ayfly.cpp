#include "decoder_ayfly.h"
#include "ayflyhelper.h"

DecoderAyfly::DecoderAyfly(const QString &path)
    : Decoder()
{
    m_helper = new AyflyHelper(path);
}

DecoderAyfly::~DecoderAyfly()
{
    delete m_helper;
}

bool DecoderAyfly::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderAyfly: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderAyfly: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderAyfly: initialize success");
    return true;
}

qint64 DecoderAyfly::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderAyfly::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderAyfly::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderAyfly::seek(qint64 time)
{
    m_helper->seek(time);
}
