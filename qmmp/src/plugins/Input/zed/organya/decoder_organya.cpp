#include "decoder_organya.h"
#include "organyahelper.h"

DecoderOrganya::DecoderOrganya(const QString &path)
    : Decoder()
{
    m_helper = new OrganyaHelper(path);
}

DecoderOrganya::~DecoderOrganya()
{
    delete m_helper;
}

bool DecoderOrganya::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderOrganya: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderOrganya: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderOrganya: initialize success");
    return true;
}

qint64 DecoderOrganya::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderOrganya::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderOrganya::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderOrganya::seek(qint64 time)
{
    m_helper->seek(time);
}
