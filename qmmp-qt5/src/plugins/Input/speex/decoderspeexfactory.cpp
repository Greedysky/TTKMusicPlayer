#include "speexhelper.h"
#include "decoder_speex.h"
#include "decoderspeexfactory.h"

bool DecoderSpeexFactory::canDecode(QIODevice *input) const
{
    SpeexHelper wrap(input);
    return wrap.initialize();
}

DecoderProperties DecoderSpeexFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SPEEX Plugin");
    properties.filters << "*.spx";
    properties.description = tr("Speex Music Files");
    properties.shortName = "speex";
    properties.hasSettings = true;
    properties.noInput = true;
    properties.protocols << "speex";
    return properties;
}

Decoder *DecoderSpeexFactory::create(const QString &, QIODevice *input)
{
    return new DecoderSpeex(input);
}

QList<TrackInfo *> DecoderSpeexFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
    {
        return QList<TrackInfo *>() << info;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        SpeexHelper wrap(&file);
        if(!wrap.initialize())
        {
            return QList<TrackInfo *>();
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, wrap.get_bitrate());
            info->setValue(Qmmp::SAMPLERATE, wrap.stream_get_samplerate());
            info->setValue(Qmmp::CHANNELS, wrap.stream_get_channels());
            info->setDuration(wrap.get_duration() / 1000);
        }
    }

    return QList<TrackInfo *>() << info;
}

MetaDataModel* DecoderSpeexFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
