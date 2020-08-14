#include "dumbhelper.h"
#include "decoder_dumb.h"

DecoderDumb::DecoderDumb(const QString &path)
    : Decoder()
{
    m_dumb = new DumbHelper(path);
}

DecoderDumb::~DecoderDumb()
{
    delete m_dumb;
}

bool DecoderDumb::initialize()
{
    if(!m_dumb->initialize())
    {
        return false;
    }

    int rate = m_dumb->samplerate();
    int channels = m_dumb->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderDumb::totalTime() const
{
    return m_dumb->totalTime();
}

int DecoderDumb::bitrate() const
{
    return m_dumb->bitrate();
}

qint64 DecoderDumb::read(unsigned char *data, qint64 size)
{
    return m_dumb->read(data, size);
}

void DecoderDumb::seek(qint64 pos)
{
    m_dumb->seek(pos);
}
