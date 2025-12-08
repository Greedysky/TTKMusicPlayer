#include "decoder_xgm.h"
#include "xgmhelper.h"

DecoderXGM::DecoderXGM(const QString &path)
    : Decoder()
{
    m_helper = new XGMHelper(path);
}

DecoderXGM::~DecoderXGM()
{
    delete m_helper;
}

bool DecoderXGM::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderXGM: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderXGM: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderXGM: initialize success");
    return true;
}

qint64 DecoderXGM::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderXGM::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderXGM::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderXGM::seek(qint64 time)
{
    m_helper->seek(time);
}
