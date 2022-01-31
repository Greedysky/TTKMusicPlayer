#include "decoder_optimfrog.h"
#include "optimfroghelper.h"

DecoderOptimFROG::DecoderOptimFROG(QIODevice *input)
    : Decoder(input)
{
}

DecoderOptimFROG::~DecoderOptimFROG()
{
    delete m_helper;
}

bool DecoderOptimFROG::initialize()
{
    if(!input())
        return false;

    if(!input()->isOpen() && !input()->open(QIODevice::ReadOnly))
        return false;

    m_helper = new OptimFROGHelper(input());
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

    configure(m_helper->sampleRate(), m_helper->channels(), format);
    qDebug("DecoderOptimFROG: initialize success");
    return true;
}

qint64 DecoderOptimFROG::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderOptimFROG::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderOptimFROG::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderOptimFROG::seek(qint64 time)
{
    m_helper->seek(time);
}
