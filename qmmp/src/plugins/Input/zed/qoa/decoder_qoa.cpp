#include "decoder_qoa.h"
#include "qoahelper.h"

DecoderQOA::DecoderQOA(const QString &path)
    : Decoder()
{
    m_helper = new QOAHelper(path);
}

DecoderQOA::~DecoderQOA()
{
    delete m_helper;
}

bool DecoderQOA::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderQOA: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderQOA: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_FLOAT);
    qDebug("DecoderQOA: initialize success");
    return true;
}

qint64 DecoderQOA::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderQOA::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderQOA::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderQOA::seek(qint64 time)
{
    m_helper->seek(time);
}
