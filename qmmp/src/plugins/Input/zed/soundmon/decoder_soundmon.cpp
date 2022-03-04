#include "decoder_soundmon.h"
#include "soundmonhelper.h"

DecoderSoundMon::DecoderSoundMon(const QString &path)
    : Decoder()
{
    m_helper = new SoundMonHelper(path);
}

DecoderSoundMon::~DecoderSoundMon()
{
    delete m_helper;
}

bool DecoderSoundMon::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderSoundMon: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderSoundMon: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_U8);
    qDebug("DecoderSoundMon: initialize success");
    return true;
}

qint64 DecoderSoundMon::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderSoundMon::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderSoundMon::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderSoundMon::seek(qint64 time)
{
    m_helper->seek(time);
}
