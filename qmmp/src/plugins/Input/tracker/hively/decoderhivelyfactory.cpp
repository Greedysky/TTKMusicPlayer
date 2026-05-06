#include "decoderhivelyfactory.h"
#include "hivelymetadatamodel.h"
#include "hivelyhelper.h"
#include "decoder_hively.h"

bool DecoderHivelyFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    HivelyHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderHivelyFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Hively Plugin");
    properties.shortName = "hively";
    properties.filters << "*.ahx" << "*.hvl";
    properties.description = "HVL Module File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderHivelyFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderHively(path);
}

TrackInfoList DecoderHivelyFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo raw(path), *info = &raw;
    if(parts == TrackInfo::Parts())
    {
        return {raw};
    }

    HivelyHelper helper(path);
    if(!helper.initialize())
    {
        return {};
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, helper.title());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "Hively");
        info->setDuration(helper.totalTime());
    }

    return {raw};
}

MetaDataModel *DecoderHivelyFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new HivelyMetaDataModel(path);
}

QDialog *DecoderHivelyFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(hively, DecoderHivelyFactory)
#endif
