#include "decoder_optimfrog.h"

DecoderOptimFROG::DecoderOptimFROG(QIODevice *device) : Decoder(device)
{
}

bool DecoderOptimFROG::initialize()
{
    if(!input())
        return false;
    if(!input()->isOpen() && !input()->open(QIODevice::ReadOnly))
        return false;

    try
    {
        m_frog = std::unique_ptr<OptimFROGWrap>(new OptimFROGWrap(input()));
    }
    catch(const OptimFROGWrap::InvalidFile &)
    {
        return false;
    }

    enum Qmmp::AudioFormat format;
    switch(m_frog->depth())
    {
        case 8:
            format = Qmmp::PCM_S8;
            break;
        case 16:
            format = Qmmp::PCM_S16LE;
            break;
        default:
            return false;
    }

    configure(m_frog->rate(), m_frog->channels(), format);

    return true;
}

qint64 DecoderOptimFROG::totalTime() const
{
    return m_frog->length();
}

int DecoderOptimFROG::bitrate() const
{
    return m_frog->bitrate();
}

qint64 DecoderOptimFROG::read(unsigned char *audio, qint64 max_size)
{
    return m_frog->read(audio, max_size);
}

void DecoderOptimFROG::seek(qint64 pos)
{
    m_frog->seek(pos);
}
