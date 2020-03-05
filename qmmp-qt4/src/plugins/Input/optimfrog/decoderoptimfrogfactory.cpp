#include "decoderoptimfrogfactory.h"
#include "decoder_optimfrog.h"
#include "optimfrogwrap.h"
#include "optimfrogmetadatamodel.h"

#include <QtPlugin>

bool DecoderOptimFROGFactory::canDecode(QIODevice *device) const
{
    try
    {
        OptimFROGWrap wrap(device);
        return true;
    }
    catch(const OptimFROGWrap::InvalidFile &)
    {
        return false;
    }
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

Decoder *DecoderOptimFROGFactory::create(const QString &, QIODevice *device)
{
    return new DecoderOptimFROG(device);
}

MetaDataModel *DecoderOptimFROGFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
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
        try
        {
            OptimFROGWrap frog(&file);

            if(parts & TrackInfo::Properties)
            {
                info->setValue(Qmmp::BITRATE, frog.bitrate());
                info->setValue(Qmmp::SAMPLERATE, frog.rate());
                info->setValue(Qmmp::CHANNELS, frog.channels());
                info->setDuration(frog.length() / 1000);
            }

            if((parts & TrackInfo::MetaData) && frog.hasTags())
            {
                QString value;
                value = QString::fromStdString(frog.getTag("title"));
                info->setValue(Qmmp::TITLE, value.replace('\n', "<br>"));
                value = QString::fromStdString(frog.getTag("artist"));
                info->setValue(Qmmp::ARTIST, value.replace('\n', "<br>"));
                value = QString::fromStdString(frog.getTag("album"));
                info->setValue(Qmmp::ALBUM, value.replace('\n', "<br>"));
                value = QString::fromStdString(frog.getTag("comment"));
                info->setValue(Qmmp::COMMENT, value.replace('\n', "<br>"));
                value = QString::fromStdString(frog.getTag("genre"));
                info->setValue(Qmmp::GENRE, value.replace('\n', "<br>"));
                value = QString::fromStdString(frog.getTag("composer"));
                info->setValue(Qmmp::COMPOSER, value.replace('\n', "<br>"));
                value = QString::fromStdString(frog.getTag("year"));
                info->setValue(Qmmp::YEAR, value.replace('\n', "<br>"));
                value = QString::fromStdString(frog.getTag("track"));
                info->setValue(Qmmp::TRACK, value.replace('\n', "<br>"));
            }
        }
        catch(const OptimFROGWrap::InvalidFile &)
        {
        }
    }

    return QList<TrackInfo *>() << info;
}

Q_EXPORT_PLUGIN2(optimfrog, DecoderOptimFROGFactory)
