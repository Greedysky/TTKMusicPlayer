#include "speexhelper.h"
#include "decoder_speex.h"
#include "decoderspeexfactory.h"

bool DecoderSpeexFactory::canDecode(QIODevice *input) const
{
    SpeexHelper helper(input);
    return helper.initialize();
}

DecoderProperties DecoderSpeexFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Speex Plugin");
    properties.filters << "*.spx";
    properties.description = tr("Speex Audio Files");
    properties.shortName = "speex";
    properties.noInput = true;
    properties.protocols << "speex";
    return properties;
}

Decoder *DecoderSpeexFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderSpeex(input);
}

QList<TrackInfo*> DecoderSpeexFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        SpeexHelper helper(&file);
        if(!helper.initialize())
        {
            delete info;
            return QList<TrackInfo*>();
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, helper.get_bitrate());
            info->setValue(Qmmp::SAMPLERATE, helper.stream_get_samplerate());
            info->setValue(Qmmp::CHANNELS, helper.stream_get_channels());
            info->setValue(Qmmp::FORMAT_NAME, "Speex");
            info->setDuration(helper.get_duration());
        }
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderSpeexFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(speex, DecoderSpeexFactory)
#endif
