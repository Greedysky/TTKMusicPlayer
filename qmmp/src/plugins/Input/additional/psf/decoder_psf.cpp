#include "psfhelper.h"
#include "decoder_psf.h"

DecoderPSF::DecoderPSF(const QString &path)
    : Decoder()
{
    m_psf = new PSFHelper(path);
}

DecoderPSF::~DecoderPSF()
{
    delete m_psf;
}

bool DecoderPSF::initialize()
{
    if(!m_psf->initialize())
    {
        return false;
    }

    int rate = m_psf->samplerate();
    int channels = m_psf->channels();
    if(rate == 0 || channels == 0)
    {
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);

    return true;
}

qint64 DecoderPSF::totalTime() const
{
    return m_psf->totalTime();
}

int DecoderPSF::bitrate() const
{
    return m_psf->bitrate();
}

qint64 DecoderPSF::read(unsigned char *data, qint64 size)
{
    return m_psf->read(data, size);
}

void DecoderPSF::seek(qint64 pos)
{
    m_psf->seek(pos);
}
