#include "replaygainreader.h"

ReplayGainReader::ReplayGainReader(AVFormatContext *ic)
{
    AVDictionaryEntry *t = nullptr;
    while((t = av_dict_get(ic->metadata, "", t, AV_DICT_IGNORE_SUFFIX)))
    {
        if(!strcmp(t->key, "replaygain_album_gain"))
            setValue(Qmmp::REPLAYGAIN_ALBUM_GAIN, t->value);
        else if(!strcmp(t->key, "replaygain_album_peak"))
            setValue(Qmmp::REPLAYGAIN_ALBUM_PEAK, t->value);
        else if(!strcmp(t->key, "replaygain_track_gain"))
            setValue(Qmmp::REPLAYGAIN_TRACK_GAIN, t->value);
        else if(!strcmp(t->key, "replaygain_track_peak"))
            setValue(Qmmp::REPLAYGAIN_TRACK_PEAK, t->value);
    }
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
    bool ok;
    double v = value.toDouble(&ok);
    if(ok)
        m_values[key] = v;
}
