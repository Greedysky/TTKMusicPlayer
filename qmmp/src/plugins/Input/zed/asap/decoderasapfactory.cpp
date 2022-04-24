#include "decoderasapfactory.h"
#include "asaphelper.h"
#include "decoder_asap.h"
#include "asapmetadatamodel.h"

bool DecoderAsapFactory::canDecode(QIODevice *input) const
{
    QFile *file = static_cast<QFile*>(input);
    if(!file)
        return false;

    AsapHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderAsapFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Asap Plugin");
    properties.shortName = "asap";
    properties.filters << "*.cm3" << "*.cmc" << "*.cmr" << "*.cms";
    properties.filters << "*.dlt" << "*.dmc";
    properties.filters << "*.fc";
    properties.filters << "*.mpd" << "*.mpt" << "*.rmt";
    properties.filters << "*.sap";
    properties.filters << "*.tm2" << "*.tm8" << "*.tmc";
    properties.description = "Another Slight Atari Player File";
    properties.protocols << "file";
    properties.noInput = true;
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
        const QMap<Qmmp::MetaData, QString> metaData(helper.readMetaData());
        for(auto itr = metaData.begin(); itr != metaData.end(); ++itr)
        {
            info->setValue(itr.key(), itr.value());
        }
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "Asap Audio");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAsapFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new AsapMetaDataModel(path);
}

void DecoderAsapFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(asap, DecoderAsapFactory)
#endif
