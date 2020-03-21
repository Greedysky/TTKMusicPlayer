#include "decoderoptimfrogfactory.h"
#include "decoder_optimfrog.h"
#include "optimfrogwrap.h"
#include "optimfrogmetadatamodel.h"

#include <QtPlugin>

bool DecoderOptimFROGFactory::canDecode(QIODevice *device) const
{
    OptimFROGWrap wrap(device);
    return wrap.initialize();
}

DecoderProperties DecoderOptimFROGFactory::properties() const
{
    DecoderProperties properties;

    properties.name = tr("OptimFROG Plugin");
    properties.shortName = "optimfrog";
    properties.filters << "*.ofr" << "*.ofs";
    properties.description = tr("OptimFROG Files");
    properties.protocols << "file";
    properties.hasSettings = false;
    properties.noInput = true;

    return properties;
}

Decoder *DecoderOptimFROGFactory::create(const QString &, QIODevice *input)
{
    return new DecoderOptimFROG(input);
}

MetaDataModel *DecoderOptimFROGFactory::createMetaDataModel(const QString &path, bool)
{
    return new OptimFROGMetaDataModel(path);
}

QList<TrackInfo *> DecoderOptimFROGFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::NoParts)
        return QList<TrackInfo *>() << info;

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        OptimFROGWrap wrap(&file);
        if(!wrap.initialize())
        {
            return QList<TrackInfo *>();
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, wrap.bitrate());
            info->setValue(Qmmp::SAMPLERATE, wrap.rate());
            info->setValue(Qmmp::CHANNELS, wrap.channels());
            info->setDuration(wrap.length() / 1000);
        }

        if((parts & TrackInfo::MetaData) && wrap.hasTags())
        {
            QString value;
            value = wrap.getTag("title");
            info->setValue(Qmmp::TITLE, value.replace('\n', "<br>"));
            value = wrap.getTag("artist");
            info->setValue(Qmmp::ARTIST, value.replace('\n', "<br>"));
            value = wrap.getTag("album");
            info->setValue(Qmmp::ALBUM, value.replace('\n', "<br>"));
            value = wrap.getTag("comment");
            info->setValue(Qmmp::COMMENT, value.replace('\n', "<br>"));
            value = wrap.getTag("genre");
            info->setValue(Qmmp::GENRE, value.replace('\n', "<br>"));
            value = wrap.getTag("composer");
            info->setValue(Qmmp::COMPOSER, value.replace('\n', "<br>"));
            value = wrap.getTag("year");
            info->setValue(Qmmp::YEAR, value.replace('\n', "<br>"));
            value = wrap.getTag("track");
            info->setValue(Qmmp::TRACK, value.replace('\n', "<br>"));
        }
    }

    return QList<TrackInfo *>() << info;
}

Q_EXPORT_PLUGIN2(optimfrog, DecoderOptimFROGFactory)
