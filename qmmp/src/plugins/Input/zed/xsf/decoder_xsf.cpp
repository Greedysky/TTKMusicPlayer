#include "xsfhelper.h"
#include "decoder_xsf.h"

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
    qDebug("DecoderXSF: initialize succes");
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

qint64 DecoderXSF::read(unsigned char *data, qint64 size)
{
    return m_helper->read(data, size);
}

void DecoderXSF::seek(qint64 pos)
{
    m_helper->seek(pos);
}
