#include "decoderpsffactory.h"
#include "psfhelper.h"
#include "decoder_psf.h"
#include "psfmetadatamodel.h"

bool DecoderPSFFactory::canDecode(QIODevice *input) const
{
    QFile *file = static_cast<QFile*>(input);
    if(!file)
        return false;

    PSFHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderPSFFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("PSF Plugin");
    properties.shortName = "psf";
    properties.filters << "*.psf" << "*.minipsf";
    properties.filters << "*.ssf" << "*.minissf";
    properties.filters << "*.qsf" << "*.miniqsf";
    properties.filters << "*.dsf" << "*.minidsf";
    properties.filters << "*.spu";
    properties.description = "Audio Overload File";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderPSFFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderPSF(path);
}

QList<TrackInfo*> DecoderPSFFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    PSFHelper helper(path);
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
        info->setValue(Qmmp::FORMAT_NAME, "PSF");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderPSFFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new PSFMetaDataModel(path);
}

void DecoderPSFFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(psf, DecoderPSFFactory)
#endif
