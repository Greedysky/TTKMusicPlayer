#include "psfhelper.h"
#include "decoder_psf.h"

DecoderPSF::DecoderPSF(const QString &path)
    : Decoder()
{
    m_helper = new PSFHelper(path);
}

DecoderPSF::~DecoderPSF()
{
    delete m_helper;
}

bool DecoderPSF::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderPSF: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderPSF: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderPSF: initialize succes");
    return true;
}

qint64 DecoderPSF::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderPSF::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderPSF::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderPSF::seek(qint64 pos)
{
    m_helper->seek(pos);
}
