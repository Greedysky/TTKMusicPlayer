#include "sc68helper.h"
#include "decoder_sc68.h"
#include "decodersc68factory.h"
#include "sc68metadatamodel.h"

bool DecoderSC68Factory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderSC68Factory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("SC68 Plugin");
    properties.filters << "*.sndh" << "*.snd" << "*.sc68";
    properties.description = tr("Atari ST(E) And Amiga Audio Files");
    properties.shortName = "sc68";
    properties.noInput = true;
    properties.protocols << "sc68";
    return properties;
}

Decoder *DecoderSC68Factory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSC68(path);
}

QList<TrackInfo*> DecoderSC68Factory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    SC68Helper helper(path);
    if(!helper.initialize())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        helper.readMetaTags();
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.artist());
        info->setValue(Qmmp::ALBUM, helper.album());
        info->setValue(Qmmp::YEAR, helper.year());
        info->setValue(Qmmp::GENRE, helper.genre());
        info->setValue(Qmmp::TRACK, helper.track());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.samplerate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.bitsPerSample());
        info->setValue(Qmmp::FORMAT_NAME, "sc68");
        info->setDuration(helper.totalTime());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderSC68Factory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(readOnly);
    return new SC68MetaDataModel(path);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(sc68, DecoderSC68Factory)
#endif
