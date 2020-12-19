#include "decoder_adplug.h"

DecoderAdplug::DecoderAdplug(const QString &path)
    : Decoder(),
      m_path(path)
{

}

bool DecoderAdplug::initialize()
{
    m_adplug = std::unique_ptr<AdplugHelper>(new AdplugHelper(qUtf8Printable(m_path)));
    if(!m_adplug->initialize())
    {
        return false;
    }
    configure(m_adplug->rate(), m_adplug->channels(), Qmmp::PCM_S16LE);

    m_length = m_adplug->length();
    m_divisor = (m_adplug->rate() * m_adplug->channels() * (m_adplug->depth() / 8)) / 1000.0;

    return true;
}

qint64 DecoderAdplug::totalTime() const
{
    return m_adplug->length();
}

int DecoderAdplug::bitrate() const
{
    return m_adplug->depth();
}

qint64 DecoderAdplug::read(unsigned char *audio, qint64 max_size)
{
    qint64 copied;

    copied = copy(audio, max_size);
    audio += copied;
    max_size -= copied;

    /* Some songs loop endlessly.  If we pass the length threshold (Adplug
    * caps the reported length at 10 minutes), then report EOF.
    */
    if(m_time > m_length)
    {
        return 0;
    }

    if(m_buf_filled == 0)
    {
        AdplugHelper::Frame frame = m_adplug->read();
        if(frame.m_n == 0)
        {
            return copied;
        }

        m_bufptr = frame.m_buf;
        m_buf_filled += frame.m_n;
    }

    copied += copy(audio, max_size);
    m_time += copied / m_divisor;
    return copied;
}

void DecoderAdplug::seek(qint64 pos)
{
    m_adplug->seek(pos);
    m_time = pos;
}

qint64 DecoderAdplug::copy(unsigned char *audio, qint64 max_size)
{
    qint64 to_copy = qMin(m_buf_filled, max_size);
    memcpy(audio, m_bufptr, to_copy);

    m_bufptr += to_copy;
    m_buf_filled -= to_copy;

    return to_copy;
}
