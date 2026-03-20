#include "audioparameters.h"

class AudioParametersPrivate
{
public:
    quint32 srate = 0;
    ChannelMap chanMap;
    Qmmp::AudioFormat format = Qmmp::PCM_S16LE;
    int sampleSize = 2;
    int validBitsPerSample = 16;

};


AudioParameters::AudioParameters()
    : d(new AudioParametersPrivate)
{

}

AudioParameters::AudioParameters(const AudioParameters &other)
    : d(new AudioParametersPrivate)
{
    operator=(other);
}

AudioParameters::AudioParameters(AudioParameters &&other) noexcept
    : d(nullptr)
{
    std::swap(d, other.d);
}

AudioParameters::AudioParameters(quint32 srate, const ChannelMap &map, Qmmp::AudioFormat format)
    : d(new AudioParametersPrivate)
{
    d->srate = srate;
    d->chanMap = map;
    d->format = format;
    d->sampleSize = sampleSize(format);
    d->validBitsPerSample = validBitsPerSample(format);
}

AudioParameters::~AudioParameters()
{
    delete d;
}

AudioParameters &AudioParameters::operator=(const AudioParameters &other)
{
    d->srate = other.sampleRate();
    d->chanMap = other.channelMap();
    d->format = other.format();
    d->sampleSize = other.sampleSize();
    d->validBitsPerSample = other.validBitsPerSample();
    return *this;
}

AudioParameters &AudioParameters::operator=(AudioParameters &&other) noexcept
{
    std::swap(d, other.d);
    return *this;
}

bool AudioParameters::operator==(const AudioParameters &other) const
{
    return d->srate == other.sampleRate() && d->chanMap == other.channelMap() && d->format == other.format() &&
           d->validBitsPerSample == other.validBitsPerSample();
}

bool AudioParameters::operator!=(const AudioParameters &other) const
{
    return !operator==(other);
}

quint32 AudioParameters::sampleRate() const
{
    return d->srate;
}

int AudioParameters::channels() const
{
    return d->chanMap.count();
}

const ChannelMap &AudioParameters::channelMap() const
{
    return d->chanMap;
}

Qmmp::AudioFormat AudioParameters::format() const
{
    return d->format;
}

int AudioParameters::sampleSize() const
{
    return d->sampleSize;
}

int AudioParameters::frameSize() const
{
    return d->sampleSize * d->chanMap.count();
}

int AudioParameters::bitsPerSample() const
{
    return d->sampleSize * 8;
}

int AudioParameters::validBitsPerSample() const
{
    return d->validBitsPerSample;
}

AudioParameters::ByteOrder AudioParameters::byteOrder() const
{
    switch(d->format)
    {
    case Qmmp::PCM_S16BE:
    case Qmmp::PCM_S24BE:
    case Qmmp::PCM_S32BE:
        return BigEndian;
    default:
        return LittleEndian;
    }
}

QString AudioParameters::toString() const
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
        if(d->format == format_names[i].format)
        {
            name = format_names[i].name;
            break;
        }
    }

    return QString("%1 Hz, {%2}, %3").arg(d->srate).arg(d->chanMap.toString(), name);
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
