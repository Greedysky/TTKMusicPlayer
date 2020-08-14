#include "asaphelper.h"
#include "decoder_asap.h"
#include "decoderasapfactory.h"
#include "asapmetadatamodel.h"

bool DecoderAsapFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAsapFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Asap Plugin");
    properties.filters << "*.sap";
    properties.filters << "*.cm3" << "*.cmc" << "*.cmr" << "*.cms";
    properties.filters << "*.dmc" << "*.dlt";
    properties.filters << "*.mpd" << "*.mpt" << "*.rmt" << "*.tm2" << "*.tm8" << "*.tmc" << "*.fc";
    properties.description = tr("Another Slight Atari Player Files");
    properties.shortName = "asap";
    properties.noInput = true;
    properties.protocols << "asap";
    return properties;
}

Decoder *DecoderAsapFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAsap(path);
}

QList<TrackInfo*> DecoderAsapFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AsapHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        helper.readMetaTags();
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::YEAR, helper.year());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.samplerate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "asap");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAsapFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new AsapMetaDataModel(path);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(asap, DecoderAsapFactory)
#endif
