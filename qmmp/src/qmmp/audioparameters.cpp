#include <QMap>
#include "audioparameters.h"

AudioParameters::AudioParameters()
{

}

AudioParameters::AudioParameters(const AudioParameters &other)
    : m_srate(other.sampleRate()),
      m_chan_map(other.channelMap()),
      m_format(other.format()),
      m_sz(other.sampleSize()),
      m_precision(other.validBitsPerSample())
{

}

AudioParameters::AudioParameters(quint32 srate, const ChannelMap &map, Qmmp::AudioFormat format)
    : m_srate(srate),
      m_chan_map(map),
      m_format(format),
      m_sz(sampleSize(format)),
      m_precision(validBitsPerSample(format))
{

}

AudioParameters &AudioParameters::operator=(const AudioParameters &p)
{
    m_srate = p.sampleRate();
    m_chan_map = p.channelMap();
    m_format = p.format();
    m_sz = p.sampleSize();
    m_precision = p.validBitsPerSample();
    return *this;
}

bool AudioParameters::operator==(const AudioParameters &p) const
{
    return m_srate == p.sampleRate() && m_chan_map == p.channelMap() && m_format == p.format()
            && m_precision == p.validBitsPerSample();
}

bool AudioParameters::operator!=(const AudioParameters &p) const
{
    return !operator==(p);
}

quint32 AudioParameters::sampleRate() const
{
    return m_srate;
}

int AudioParameters::channels() const
{
    return m_chan_map.count();
}

const ChannelMap AudioParameters::channelMap() const
{
    return m_chan_map;
}

Qmmp::AudioFormat AudioParameters::format() const
{
    return m_format;
}

int AudioParameters::sampleSize() const
{
    return m_sz;
}

int AudioParameters::frameSize() const
{
    return m_sz * m_chan_map.count();
}

int AudioParameters::bitsPerSample() const
{
    return m_sz * 8;
}

int AudioParameters::validBitsPerSample() const
{
    return m_precision;
}

AudioParameters::ByteOrder AudioParameters::byteOrder() const
{
    switch(m_format)
    {
    case Qmmp::PCM_S16BE:
    case Qmmp::PCM_S24BE:
    case Qmmp::PCM_S32BE:
        return BigEndian;
    default:
        return LittleEndian;
    }
}

const QString AudioParameters::toString() const
{
    static const struct
    {
        Qmmp::AudioFormat format;
        QString name;
    }
    format_names [] =
    {
    { Qmmp::PCM_S8, "s8" },
    { Qmmp::PCM_U8, "u8" },
    { Qmmp::PCM_S16LE, "s16le" },
    { Qmmp::PCM_S16BE, "s16be" },
    { Qmmp::PCM_U16LE, "u16le" },
    { Qmmp::PCM_U16BE, "u16be" },
    { Qmmp::PCM_S24LE, "s24le" },
    { Qmmp::PCM_S24BE, "s24be" },
    { Qmmp::PCM_U24LE, "u24le" },
    { Qmmp::PCM_U24BE, "u24be" },
    { Qmmp::PCM_S32LE, "s32le" },
    { Qmmp::PCM_S32BE, "s32be" },
    { Qmmp::PCM_U32LE, "u32le" },
    { Qmmp::PCM_U32BE, "u32be" },
    { Qmmp::PCM_FLOAT, "float" },
    { Qmmp::PCM_UNKNOWN, QString() }
    };

    QString name = "unknown";
    for(int i = 0; format_names[i].format != Qmmp::PCM_UNKNOWN; ++i)
    {
        if(m_format == format_names[i].format)
        {
            name = format_names[i].name;
            break;
        }
    }

    return QString("%1 Hz, {%2}, %3").arg(m_srate).arg(m_chan_map.toString(), name);
}

int AudioParameters::sampleSize(Qmmp::AudioFormat format)
{
    switch(format)
    {
    case Qmmp::PCM_S8:
    case Qmmp::PCM_U8:
        return 1;
    case Qmmp::PCM_S16LE:
    case Qmmp::PCM_S16BE:
    case Qmmp::PCM_U16LE:
    case Qmmp::PCM_U16BE:
        return 2;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S24BE:
    case Qmmp::PCM_U24LE:
    case Qmmp::PCM_U24BE:
    case Qmmp::PCM_S32LE:
    case Qmmp::PCM_S32BE:
    case Qmmp::PCM_U32LE:
    case Qmmp::PCM_U32BE:
    case Qmmp::PCM_FLOAT:
        return 4;
    default:
        return 0;
    }
}

int AudioParameters::bitsPerSample(Qmmp::AudioFormat format)
{
    return sampleSize(format) * 8;
}

int AudioParameters::validBitsPerSample(Qmmp::AudioFormat format)
{
    switch(format)
    {
    case Qmmp::PCM_S8:
    case Qmmp::PCM_U8:
        return 8;
    case Qmmp::PCM_S16LE:
    case Qmmp::PCM_S16BE:
    case Qmmp::PCM_U16LE:
    case Qmmp::PCM_U16BE:
        return 16;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S24BE:
    case Qmmp::PCM_U24LE:
    case Qmmp::PCM_U24BE:
        return 24;
    case Qmmp::PCM_S32LE:
    case Qmmp::PCM_S32BE:
    case Qmmp::PCM_U32LE:
    case Qmmp::PCM_U32BE:
    case Qmmp::PCM_FLOAT:
        return 32;
    default:
        return 0;
    }
}

Qmmp::AudioFormat AudioParameters::findAudioFormat(int bits, ByteOrder byteOrder)
{
    switch(bits)
    {
    case 8:
        return Qmmp::PCM_U8;
    case 16:
        return (byteOrder == LittleEndian) ? Qmmp::PCM_U16LE : Qmmp::PCM_U16BE;
    case 24:
        return (byteOrder == LittleEndian) ? Qmmp::PCM_U24LE : Qmmp::PCM_U24BE;
    case 32:
        return (byteOrder == LittleEndian) ? Qmmp::PCM_U32LE : Qmmp::PCM_U32BE;
    default:
        return Qmmp::PCM_UNKNOWN;
    }
}
