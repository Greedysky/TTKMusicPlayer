#include "dcahelper.h"
#include "decoder_dca.h"

DecoderDCA::DecoderDCA(const QString &path)
    : Decoder()
{
    m_dca = new DCAHelper(path);
}

DecoderDCA::~DecoderDCA()
{
    delete m_dca;
}

bool DecoderDCA::initialize()
{
    if(!m_dca->initialize())
    {
        qWarning("DecoderDCA: initialize failed");
        return false;
    }

    int rate = m_dca->sampleRate();
    int channels = m_dca->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderDCA: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderDCA::totalTime() const
{
    return m_dca->totalTime();
}

int DecoderDCA::bitrate() const
{
    return m_dca->bitrate();
}

qint64 DecoderDCA::read(unsigned char *data, qint64 size)
{
    return m_dca->read(data, size);
}

void DecoderDCA::seek(qint64 pos)
{
    m_dca->seek(pos);
}
