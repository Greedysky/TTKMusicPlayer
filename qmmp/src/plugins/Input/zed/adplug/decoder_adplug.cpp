#include "decoder_adplug.h"

DecoderAdplug::DecoderAdplug(const QString &path)
    : Decoder()
{
    m_helper = new AdplugHelper(path);
}

DecoderAdplug::~DecoderAdplug()
{
    delete m_helper;
}

bool DecoderAdplug::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderAdplug: initialize failed");
        return false;
    }

    m_length = m_helper->length();
    m_divisor = (m_helper->rate() * m_helper->channels() * (m_helper->depth() / 8)) / 1000.0;

    configure(m_helper->rate(), m_helper->channels(), Qmmp::PCM_S16LE);
    qDebug("DecoderAdplug: initialize success");
    return true;
}

qint64 DecoderAdplug::totalTime() const
{
    return m_helper->length();
}

int DecoderAdplug::bitrate() const
{
    return m_helper->depth();
}

qint64 DecoderAdplug::read(unsigned char *data, qint64 maxSize)
{
    qint64 copied = copy(data, maxSize);
    data += copied;
    maxSize -= copied;

    /* Some songs loop endlessly.  If we pass the length threshold (Adplug
    * caps the reported length at 10 minutes), then report EOF.
    */
    if(m_time > m_length)
    {
        return 0;
    }

    if(m_buf_filled == 0)
    {
        AdplugHelper::Frame frame = m_helper->read();
        if(frame.m_n == 0)
        {
            return copied;
        }

        m_bufptr = frame.m_buf;
        m_buf_filled += frame.m_n;
    }

    copied += copy(data, maxSize);
    m_time += copied / m_divisor;
    return copied;
}

void DecoderAdplug::seek(qint64 time)
{
    m_helper->seek(time);
    m_time = time;
}

qint64 DecoderAdplug::copy(unsigned char *data, qint64 maxSize)
{
    const qint64 copied = qMin(m_buf_filled, maxSize);
    memcpy(data, m_bufptr, copied);

    m_bufptr += copied;
    m_buf_filled -= copied;

    return copied;
}
