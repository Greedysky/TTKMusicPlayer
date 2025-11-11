#include "decoderoptimfrogfactory.h"
#include "decoder_optimfrog.h"
#include "optimfroghelper.h"
#include "optimfrogmetadatamodel.h"

bool DecoderOptimFROGFactory::canDecode(QIODevice *input) const
{
    OptimFROGHelper helper(input);
    return helper.initialize();
}

DecoderProperties DecoderOptimFROGFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("OptimFROG Plugin");
    properties.shortName = "optimfrog";
    properties.filters << "*.ofr" << "*.ofs";
    properties.description = "OptimFROG Lossless Audio File";
    return properties;
}

Decoder *DecoderOptimFROGFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderOptimFROG(input);
}

QList<TrackInfo*> DecoderOptimFROGFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        delete info;
        return QList<TrackInfo*>();
    }

    OptimFROGHelper helper(&file);
    if(!helper.initialize())
    {
        file.close();
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "OptimFROG");
        info->setDuration(helper.totalTime());
    }

    if((parts & TrackInfo::MetaData) && helper.hasMetaData())
    {
        QString value;
        value = helper.metaData("title");
        info->setValue(Qmmp::TITLE, value.replace('\n', "<br>"));
        value = helper.metaData("artist");
        info->setValue(Qmmp::ARTIST, value.replace('\n', "<br>"));
        value = helper.metaData("album");
        info->setValue(Qmmp::ALBUM, value.replace('\n', "<br>"));
        value = helper.metaData("comment");
        info->setValue(Qmmp::COMMENT, value.replace('\n', "<br>"));
        value = helper.metaData("genre");
        info->setValue(Qmmp::GENRE, value.replace('\n', "<br>"));
        value = helper.metaData("composer");
        info->setValue(Qmmp::COMPOSER, value.replace('\n', "<br>"));
        value = helper.metaData("year");
        info->setValue(Qmmp::YEAR, value.replace('\n', "<br>"));
        value = helper.metaData("track");
        info->setValue(Qmmp::TRACK, value.replace('\n', "<br>"));
    }

    file.close();
    return QList<TrackInfo*>() << info;
}

MetaDataModel *DecoderOptimFROGFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new OptimFROGMetaDataModel(path);
}

QDialog *DecoderOptimFROGFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(optimfrog, DecoderOptimFROGFactory)
#endif
