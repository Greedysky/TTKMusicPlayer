#include "decoder_xmdx.h"
#include "xmdxhelper.h"

DecoderXMDX::DecoderXMDX(const QString &path)
    : Decoder()
{
    m_helper = new XMDXHelper(path);
}

DecoderXMDX::~DecoderXMDX()
{
    delete m_helper;
}

bool DecoderXMDX::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderXMDX: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderXMDX: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderXMDX: initialize success");
    return true;
}

qint64 DecoderXMDX::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderXMDX::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderXMDX::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderXMDX::seek(qint64 time)
{
    Q_UNUSED(time);
}
