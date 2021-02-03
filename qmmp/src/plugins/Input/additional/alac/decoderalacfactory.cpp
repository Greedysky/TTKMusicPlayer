#include "decoderalacfactory.h"
#include "decoder_alac.h"
#include "alachelper.h"

bool DecoderAlacFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAlacFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Alac Plugin");
    properties.filters << "*.alac";
    properties.description = tr("Apple Lossless Audio Codec Audio Files");
    properties.shortName = "alac";
    properties.hasSettings = false;
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderAlacFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAlac(path);
}

QList<TrackInfo*> DecoderAlacFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AlacHelper helper(qUtf8Printable(path));
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::FORMAT_NAME, "Alac");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAlacFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(alac, DecoderAlacFactory)
#endif
