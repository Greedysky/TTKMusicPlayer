#include "decoder_nezplug.h"
#include "nezplughelper.h"

DecoderNEZplug::DecoderNEZplug(const QString &path)
    : Decoder()
{
    m_helper = new NEZplugHelper(path);
}

DecoderNEZplug::~DecoderNEZplug()
{
    delete m_helper;
}

bool DecoderNEZplug::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderNEZplug: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderNEZplug: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderNEZplug: initialize success");
    return true;
}

qint64 DecoderNEZplug::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderNEZplug::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderNEZplug::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderNEZplug::seek(qint64 time)
{
    Q_UNUSED(time);
}
