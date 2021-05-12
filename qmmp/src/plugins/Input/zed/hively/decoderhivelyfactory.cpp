#include "hivelyhelper.h"
#include "decoder_hively.h"
#include "decoderhivelyfactory.h"

bool DecoderHivelyFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderHivelyFactory::properties() const
{
    DecoderProperties properties;
    properties.name = "Hively Plugin";
    properties.shortName = "hively";
    properties.filters << "*.ahx" << "*.hvl";
    properties.description = "HVL Module Files";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderHivelyFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderHively(path);
}

QList<TrackInfo*> DecoderHivelyFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    HivelyHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "Hively");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderHivelyFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderHivelyFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(hively, DecoderHivelyFactory)
#endif
