#include "decoder_stsound.h"
#include "stsoundhelper.h"

DecoderStSound::DecoderStSound(const QString &path)
    : Decoder()
{
    m_helper = new StSoundHelper(path);
}

DecoderStSound::~DecoderStSound()
{
    delete m_helper;
}

bool DecoderStSound::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderStSound: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderStSound: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderStSound: initialize success");
    return true;
}

qint64 DecoderStSound::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderStSound::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderStSound::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderStSound::seek(qint64 time)
{
    m_helper->seek(time);
}
