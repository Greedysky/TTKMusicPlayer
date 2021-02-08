#include "ayflyhelper.h"
#include "decoder_ayfly.h"

DecoderAyfly::DecoderAyfly(const QString &path)
    : Decoder()
{
    m_ayfly = new AyflyHelper(path);
}

DecoderAyfly::~DecoderAyfly()
{
    delete m_ayfly;
}

bool DecoderAyfly::initialize()
{
    if(!m_ayfly->initialize())
    {
        qWarning("DecoderAyfly: initialize failed");
        return false;
    }

    int rate = m_ayfly->sampleRate();
    int channels = m_ayfly->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderAyfly: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderAyfly::totalTime() const
{
    return m_ayfly->totalTime();
}

int DecoderAyfly::bitrate() const
{
    return m_ayfly->bitrate();
}

qint64 DecoderAyfly::read(unsigned char *data, qint64 size)
{
    return m_ayfly->read(data, size);
}

void DecoderAyfly::seek(qint64 pos)
{
    m_ayfly->seek(pos);
}
