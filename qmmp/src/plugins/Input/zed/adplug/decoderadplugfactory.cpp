#include "decoderadplugfactory.h"
#include "adplughelper.h"
#include "decoder_adplug.h"
#include "adplugmetadatamodel.h"

bool DecoderAdplugFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAdplugFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AdPlug Plugin");
    properties.shortName = "adplug";
    properties.filters << "*.a2m" << "*.adl" << "*.adlib" << "*.agd" << "*.amd" << "*.as3m";
    properties.filters << "*.bam" << "*.bmf";
    properties.filters << "*.cff" << "*.cmf";
    properties.filters << "*.d00" << "*.dfm" << "*.dmo" << "*.dro" << "*.dtm";
    properties.filters << "*.got";
    properties.filters << "*.hsc" << "*.hsq";
    properties.filters << "*.imf" << "*.ims";
    properties.filters << "*.jbm";
    properties.filters << "*.laa" << "*.lds";
    properties.filters << "*.m" << "*.mad" << "*.mkj" << "*.msc" << "*.mtk";
    properties.filters << "*.rad" << "*.raw";
    properties.filters << "*.sa2" << "*.sat" << "*.sdb" << "*.sng" << "*.sop" << "*.sqx";
    properties.filters << "*.xad" << "*.xms" << "*.xsm";
    properties.description = "AdLib Sound File";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderAdplugFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAdplug(path);
}

QList<TrackInfo*> DecoderAdplugFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AdplugHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.author());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "AdPlug");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel *DecoderAdplugFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new AdplugMetaDataModel(path);
}

void DecoderAdplugFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(adplug, DecoderAdplugFactory)
#endif
