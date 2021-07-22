#include "mdxhelper.h"
#include "decoder_mdx.h"

DecoderMdx::DecoderMdx(const QString &path)
    : Decoder()
{
    m_helper = new MdxHelper(path);
}

DecoderMdx::~DecoderMdx()
{
    delete m_helper;
}

bool DecoderMdx::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderMdx: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderMdx: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderMdx: initialize succes");
    return true;
}

qint64 DecoderMdx::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderMdx::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderMdx::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderMdx::seek(qint64 pos)
{
    m_helper->seek(pos);
}
