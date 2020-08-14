#include "speexhelper.h"
#include "decoder_speex.h"

DecoderSpeex::DecoderSpeex(QIODevice *i)
    : Decoder(i)
{
    m_speex = new SpeexHelper(i);
}

DecoderSpeex::~DecoderSpeex()
{
    delete m_speex;
}

bool DecoderSpeex::initialize()
{
    if(!input())
        return false;
    if(!input()->isOpen() && !input()->open(QIODevice::ReadOnly))
        return false;

    if(!m_speex->initialize())
    {
        return false;
    }

    int32_t rate = m_speex->stream_get_samplerate();
    int32_t channels = m_speex->stream_get_channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderSpeex::totalTime() const
{
    return m_speex->get_samples();
}

int DecoderSpeex::bitrate() const
{
    return m_speex->get_bitrate();
}

qint64 DecoderSpeex::read(unsigned char *data, qint64 size)
{
    return m_speex->read(data, size);
}

void DecoderSpeex::seek(qint64 pos)
{
    m_speex->seek(pos);
}
