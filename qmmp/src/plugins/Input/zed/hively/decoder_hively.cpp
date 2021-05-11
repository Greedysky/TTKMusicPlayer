#include "hivelyhelper.h"
#include "decoder_hively.h"

DecoderHively::DecoderHively(const QString &path)
    : Decoder()
{
    m_hively = new HivelyHelper(path);
}

DecoderHively::~DecoderHively()
{
    delete m_hively;
}

bool DecoderHively::initialize()
{
    if(!m_hively->initialize())
    {
        qWarning("DecoderHively: initialize failed");
        return false;
    }

    const int rate = m_hively->sampleRate();
    const int channels = m_hively->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderHively: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderHively: initialize succes");
    return true;
}

qint64 DecoderHively::totalTime() const
{
    return m_hively->totalTime();
}

int DecoderHively::bitrate() const
{
    return m_hively->bitrate();
}

qint64 DecoderHively::read(unsigned char *data, qint64 size)
{
    return m_hively->read(data, size);
}

void DecoderHively::seek(qint64 pos)
{
    m_hively->seek(pos);
}
