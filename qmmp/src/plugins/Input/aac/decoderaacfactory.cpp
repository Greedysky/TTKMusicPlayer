#include <QFile>
#include "aacfile.h"
#include "decoder_aac.h"
#include "decoderaacfactory.h"

bool DecoderAACFactory::canDecode(QIODevice *input) const
{
    AACFile aac_file(input, false, false);
    return aac_file.isValid();
}

DecoderProperties DecoderAACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = "AAC Plugin";
    properties.shortName = "aac";
    properties.filters << "*.aac";
    properties.description = "MPEG-4 and MPEG-2 AAC Audio Files";
    properties.contentTypes << "audio/aacp" << "audio/aac";
    return properties;
}

Decoder *DecoderAACFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderAAC(input);
}

QList<TrackInfo*> DecoderAACFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
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
        return QList<TrackInfo*>() << info;
    }

    AACFile aac_file(&file, parts & TrackInfo::MetaData);

    if(!aac_file.isValid())
    {
        delete info;
        return QList<TrackInfo*>();
    }

    if(parts & TrackInfo::MetaData)
    {
        info->setValues(aac_file.metaData());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, aac_file.bitrate());
        info->setValue(Qmmp::SAMPLERATE, aac_file.sampleRate());
        info->setValue(Qmmp::FORMAT_NAME, "AAC");
        info->setDuration(aac_file.duration());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAACFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderAACFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(aac, DecoderAACFactory)
#endif
