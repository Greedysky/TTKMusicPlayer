#include "replaygainreader.h"

ReplayGainReader::ReplayGainReader(AVFormatContext *ic)
{
    AVDictionaryEntry *t = nullptr;
    while((t = av_dict_get(ic->metadata, "REPLAYGAIN_ALBUM_GAIN", t, 0)))
        setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, QString::fromLatin1(t->value));

    t = nullptr;
    while((t = av_dict_get(ic->metadata, "REPLAYGAIN_ALBUM_PEAK", t, 0)))
        setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, QString::fromLatin1(t->value));

    t = nullptr;
    while((t = av_dict_get(ic->metadata, "REPLAYGAIN_TRACK_GAIN", t, 0)))
        setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, QString::fromLatin1(t->value));

    t = nullptr;
    while((t = av_dict_get(ic->metadata, "REPLAYGAIN_TRACK_PEAK", t, 0)))
        setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, QString::fromLatin1(t->value));
}

QMap<Qmmp::ReplayGainKey, double> ReplayGainReader::replayGainInfo() const
{
    return m_values;
}

void ReplayGainReader::setValue(Qmmp::ReplayGainKey key, QString value)
{
    value.remove(" dB");
    if(value.isEmpty())
        return;
    bool ok = false;
    double v = value.toDouble(&ok);
    if(ok)
        m_values[key] = v;
}
