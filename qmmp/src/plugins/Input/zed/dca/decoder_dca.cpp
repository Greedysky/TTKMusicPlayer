#include "dcahelper.h"
#include "decoder_dca.h"

DecoderDCA::DecoderDCA(const QString &path)
    : Decoder()
{
    m_helper = new DCAHelper(path);
}

DecoderDCA::~DecoderDCA()
{
    delete m_helper;
}

bool DecoderDCA::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderDCA: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderDCA: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderDCA: initialize succes");
    return true;
}

qint64 DecoderDCA::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderDCA::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderDCA::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderDCA::seek(qint64 pos)
{
    m_helper->seek(pos);
}
