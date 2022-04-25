#include "decoder_mdx.h"
#include "mdxhelper.h"

DecoderMDX::DecoderMDX(const QString &path)
    : Decoder()
{
    m_helper = new MDXHelper(path);
}

DecoderMDX::~DecoderMDX()
{
    delete m_helper;
}

bool DecoderMDX::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderMDX: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderMDX: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderMDX: initialize success");
    return true;
}

qint64 DecoderMDX::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderMDX::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderMDX::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderMDX::seek(qint64 time)
{
    Q_UNUSED(time);
}
