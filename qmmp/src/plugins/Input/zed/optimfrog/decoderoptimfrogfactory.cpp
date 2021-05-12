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
    properties.name = "OptimFROG Plugin";
    properties.shortName = "optimfrog";
    properties.filters << "*.ofr" << "*.ofs";
    properties.description = "OptimFROG Lossless Audio Compression Audio Files";
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
    if(file.open(QIODevice::ReadOnly))
    {
        OptimFROGHelper helper(&file);
        if(!helper.initialize())
        {
            delete info;
            return QList<TrackInfo*>();
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, helper.bitrate());
            info->setValue(Qmmp::SAMPLERATE, helper.rate());
            info->setValue(Qmmp::CHANNELS, helper.channels());
            info->setValue(Qmmp::FORMAT_NAME, "OptimFROG");
            info->setDuration(helper.length());
        }

        if((parts & TrackInfo::MetaData) && helper.hasTags())
        {
            QString value;
            value = helper.getTag("title");
            info->setValue(Qmmp::TITLE, value.replace('\n', "<br>"));
            value = helper.getTag("artist");
            info->setValue(Qmmp::ARTIST, value.replace('\n', "<br>"));
            value = helper.getTag("album");
            info->setValue(Qmmp::ALBUM, value.replace('\n', "<br>"));
            value = helper.getTag("comment");
            info->setValue(Qmmp::COMMENT, value.replace('\n', "<br>"));
            value = helper.getTag("genre");
            info->setValue(Qmmp::GENRE, value.replace('\n', "<br>"));
            value = helper.getTag("composer");
            info->setValue(Qmmp::COMPOSER, value.replace('\n', "<br>"));
            value = helper.getTag("year");
            info->setValue(Qmmp::YEAR, value.replace('\n', "<br>"));
            value = helper.getTag("track");
            info->setValue(Qmmp::TRACK, value.replace('\n', "<br>"));
        }
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel *DecoderOptimFROGFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new OptimFROGMetaDataModel(path);
}

void DecoderOptimFROGFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(optimfrog, DecoderOptimFROGFactory)
#endif
