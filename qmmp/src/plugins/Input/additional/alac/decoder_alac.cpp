#include "decoder_alac.h"
#include "alachelper.h"

DecoderAlac::DecoderAlac(const QString &path)
    : Decoder()
{
    m_alac = new AlacHelper(path);
}

DecoderAlac::~DecoderAlac()
{
    delete m_alac;
}

bool DecoderAlac::initialize()
{
    if(!m_alac->initialize())
    {
        qDebug("ALACHelper: initialize false");
        return false;
    }

    configure(m_alac->samplerate(), 2, Qmmp::PCM_S16LE);

    qDebug("ALACHelper: initialize success");
    return true;
}

qint64 DecoderAlac::totalTime() const
{
    return m_alac->totalTime();
}

int DecoderAlac::bitrate() const
{
    return m_alac->bitrate();
}

qint64 DecoderAlac::read(unsigned char *data, qint64 maxSize)
{
    return m_alac->read(data, maxSize);
}

void DecoderAlac::seek(qint64 time)
{
    m_alac->seek(time);
}
