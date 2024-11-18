#include "decoder_buzzic.h"
#include "buzzichelper.h"

DecoderBuzzic::DecoderBuzzic(const QString &path)
    : Decoder()
{
    m_helper = new BuzzicHelper(path);
}

DecoderBuzzic::~DecoderBuzzic()
{
    delete m_helper;
}

bool DecoderBuzzic::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderBuzzic: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderBuzzic: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_FLOAT);
    qDebug("DecoderBuzzic: initialize success");
    return true;
}

qint64 DecoderBuzzic::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderBuzzic::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderBuzzic::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderBuzzic::seek(qint64 time)
{
    Q_UNUSED(time);
}
