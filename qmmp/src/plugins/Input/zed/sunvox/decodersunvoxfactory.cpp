#include "decodersunvoxfactory.h"
#include "decoder_sunvox.h"
#include "sunvoxhelper.h"
#include "sunvoxmetadatamodel.h"

bool DecoderSunVoxFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    SunVoxHelper helper(file->fileName());
    return helper.initialize(true);
}

DecoderProperties DecoderSunVoxFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SunVox Plugin");
    properties.shortName = "sunvox";
    properties.filters << "*.sunvox";
    properties.description = "SunVox Library Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderSunVoxFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSunVox(path);
}

QList<TrackInfo*> DecoderSunVoxFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    SunVoxHelper helper(path);
    if(!helper.initialize(true))
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, helper.title());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "SunVox");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel *DecoderSunVoxFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new SunVoxMetaDataModel(path);
}

QDialog *DecoderSunVoxFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sunvox, DecoderSunVoxFactory)
#endif
