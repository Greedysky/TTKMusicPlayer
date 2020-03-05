#include "decoder_alac.h"
#include "alacwrap.h"

DecoderALAC::DecoderALAC(const QString &path)
    : Decoder()
{
    m_alac = new ALACWrap(path);
}

DecoderALAC::~DecoderALAC()
{
    delete m_alac;
}

bool DecoderALAC::initialize()
{
    if(!m_alac->initialize())
    {
        qDebug("ALACWrap: initialize false");
        return false;
    }

    configure(m_alac->samplerate(), 2, Qmmp::PCM_S16LE);

    qDebug("ALACWrap: initialize success");
    return true;
}

qint64 DecoderALAC::totalTime() const
{
    return m_alac->totalTime();
}

int DecoderALAC::bitrate() const
{
    return m_alac->bitrate();
}

qint64 DecoderALAC::read(unsigned char *data, qint64 size)
{
    return m_alac->read(data, size);
}

void DecoderALAC::seek(qint64 pos)
{
    m_alac->seek(pos);
}
