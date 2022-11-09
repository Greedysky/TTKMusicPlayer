#include "decoder_adplug.h"
#include "adplughelper.h"

DecoderAdPlug::DecoderAdPlug(const QString &path)
    : Decoder()
{
    m_helper = new AdPlugHelper(path);
}

DecoderAdPlug::~DecoderAdPlug()
{
    delete m_helper;
}

bool DecoderAdPlug::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderAdPlug: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderAdPlug: rate or channel invalid");
        return false;
    }

    m_length = m_helper->totalTime();
    m_divisor = (rate * channels * (m_helper->depth() / 8)) / 1000.0;

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderAdPlug: initialize success");
    return true;
}

qint64 DecoderAdPlug::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderAdPlug::bitrate() const
{
    return m_helper->depth();
}

qint64 DecoderAdPlug::read(unsigned char *data, qint64 maxSize)
{
    qint64 copied = copy(data, maxSize);
    data += copied;
    maxSize -= copied;

    /* Some songs loop endlessly.  If we pass the length threshold (AdPlug
    * caps the reported length at 10 minutes), then report EOF.
    */
    if(m_time > m_length)
    {
        return 0;
    }

    if(m_buf_filled == 0)
    {
        AdPlugHelper::Frame frame = m_helper->read();
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

void DecoderAdPlug::seek(qint64 time)
{
    m_helper->seek(time);
    m_time = time;
}

qint64 DecoderAdPlug::copy(unsigned char *data, qint64 maxSize)
{
    const qint64 copied = qMin(m_buf_filled, maxSize);
    memcpy(data, m_bufptr, copied);

    m_bufptr += copied;
    m_buf_filled -= copied;
    return copied;
}
