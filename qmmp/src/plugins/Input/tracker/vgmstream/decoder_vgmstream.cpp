#include "decoder_vgmstream.h"
#include "vgmstreamhelper.h"

DecoderVgmstream::DecoderVgmstream(const QString &path)
    : Decoder()
{
    m_helper = new VgmstreamHelper(path);
}

DecoderVgmstream::~DecoderVgmstream()
{
    delete m_helper;
}

bool DecoderVgmstream::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderVgmstream: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderVgmstream: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderVgmstream: initialize success");
    return true;
}

qint64 DecoderVgmstream::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderVgmstream::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderVgmstream::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderVgmstream::seek(qint64 time)
{
    m_helper->seek(time);
}
