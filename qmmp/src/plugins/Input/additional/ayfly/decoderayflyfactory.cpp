#include "ayflyhelper.h"
#include "decoder_ayfly.h"
#include "decoderayflyfactory.h"

bool DecoderAyflyFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAyflyFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("ALFLY Plugin");
    properties.filters << "*.vtx" << "*.asc" << "*.sqt" << "*psg";
    properties.filters << "*.stc" << "*.stp";
    properties.filters << "*.pt1" << "*.pt2" << "*.pt3" << "*.psc";
    properties.filters << "*.ay" << "*.ym";
    properties.description = tr("AY/YM Audio Files");
    properties.shortName = "alfly";
    properties.noInput = true;
    properties.protocols << "alfly";
    return properties;
}

Decoder *DecoderAyflyFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAyfly(path);
}

QList<TrackInfo*> DecoderAyflyFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AyflyHelper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        helper.readMetaTags();
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::TITLE, helper.title());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "ALFLY");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAyflyFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(alfly, DecoderAyflyFactory)
#endif
