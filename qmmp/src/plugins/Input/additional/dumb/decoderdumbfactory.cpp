#include "dumbhelper.h"
#include "decoder_dumb.h"
#include "decoderdumbfactory.h"
#include "dumbmetadatamodel.h"

bool DecoderDumbFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderDumbFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Dumb Plugin");

    const char **exts = dumb_exts();
    for(const char *q = *exts; q != nullptr;)
    {
        properties.filters << QString("*.") + q;
        q = *(++exts);
    }
    properties.description = tr("Dynamic Universal Music Bibliotheque Files");
    properties.shortName = "dumb";
    properties.noInput = true;
    properties.protocols << "dumb";
    return properties;
}

Decoder *DecoderDumbFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderDumb(path);
}

QList<TrackInfo*> DecoderDumbFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    DumbHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.samplerate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "dumb");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderDumbFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new DumbMetaDataModel(path);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(dumb, DecoderDumbFactory)
#endif
