#include "decoder_optimfrog.h"

DecoderOptimFROG::DecoderOptimFROG(QIODevice *input)
    : Decoder(input)
{
}

bool DecoderOptimFROG::initialize()
{
    if(!input())
        return false;

    if(!input()->isOpen() && !input()->open(QIODevice::ReadOnly))
        return false;

    m_helper = std::unique_ptr<OptimFROGHelper>(new OptimFROGHelper(input()));
    if(!m_helper->initialize())
    {
        qWarning("DecoderOptimFROG: initialize failed");
        return false;
    }

    enum Qmmp::AudioFormat format;
    switch(m_helper->depth())
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

    configure(m_helper->rate(), m_helper->channels(), format);
    qDebug("DecoderOptimFROG: initialize succes");
    return true;
}

qint64 DecoderOptimFROG::totalTime() const
{
    return m_helper->length();
}

int DecoderOptimFROG::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderOptimFROG::read(unsigned char *audio, qint64 max_size)
{
    return m_helper->read(audio, max_size);
}

void DecoderOptimFROG::seek(qint64 pos)
{
    m_helper->seek(pos);
}
