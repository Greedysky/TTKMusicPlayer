#include "decoder_uade.h"
#include "uadehelper.h"

DecoderUADE::DecoderUADE(const QString &path)
    : Decoder()
{
    m_helper = new UADEHelper(path);
}

DecoderUADE::~DecoderUADE()
{
    delete m_helper;
}

bool DecoderUADE::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderUADE: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderUADE: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderUADE: initialize success");
    return true;
}

qint64 DecoderUADE::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderUADE::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderUADE::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderUADE::seek(qint64 time)
{
    m_helper->seek(time);
}
