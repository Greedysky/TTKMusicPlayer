#include "decoder_xsf.h"
#include "xsfhelper.h"

DecoderXSF::DecoderXSF(const QString &path)
    : Decoder()
{
    m_helper = new XSFHelper(path);
}

DecoderXSF::~DecoderXSF()
{
    delete m_helper;
}

bool DecoderXSF::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderXSF: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderXSF: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderXSF: initialize success");
    return true;
}

qint64 DecoderXSF::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderXSF::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderXSF::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderXSF::seek(qint64 time)
{
    m_helper->seek(time);
}
