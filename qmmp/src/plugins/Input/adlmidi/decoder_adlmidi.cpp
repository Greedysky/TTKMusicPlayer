#include "decoder_adlmidi.h"
#include "adlmidihelper.h"

DecoderAdlMidi::DecoderAdlMidi(const QString &path)
    : Decoder()
{
    m_helper = new AdlMidiHelper(path);
}

DecoderAdlMidi::~DecoderAdlMidi()
{
    delete m_helper;
}

bool DecoderAdlMidi::initialize()
{
    if(!m_helper->initialize())
    {
        qWarning("DecoderAdlMidi: initialize failed");
        return false;
    }

    const int rate = m_helper->sampleRate();
    const int channels = m_helper->channels();
    if(rate == 0 || channels == 0)
    {
        qWarning("DecoderAdlMidi: rate or channel invalid");
        return false;
    }

    configure(rate, channels, Qmmp::PCM_S16LE);
    qDebug("DecoderAdlMidi: initialize success");
    return true;
}

qint64 DecoderAdlMidi::totalTime() const
{
    return m_helper->totalTime();
}

int DecoderAdlMidi::bitrate() const
{
    return m_helper->bitrate();
}

qint64 DecoderAdlMidi::read(unsigned char *data, qint64 maxSize)
{
    return m_helper->read(data, maxSize);
}

void DecoderAdlMidi::seek(qint64 time)
{
    m_helper->seek(time);
}
