#include "decoderaacfactory.h"
#include "decoder_aac.h"
#include "aacfile.h"

#include <QFile>

bool DecoderAACFactory::canDecode(QIODevice *input) const
{
    AACFile aac_file(input, false, false);
    return aac_file.isValid();
}

DecoderProperties DecoderAACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AAC Plugin");
    properties.shortName = "aac";
    properties.filters << "*.aac";
    properties.description = "MPEG-2(4) AAC Audio File";
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
        return QList<TrackInfo*>();
    }

    AACFile aac_file(&file, parts & TrackInfo::MetaData);
    if(!aac_file.isValid())
    {
        file.close();
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
        info->setValue(Qmmp::CHANNELS, 2);
        info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
        info->setValue(Qmmp::FORMAT_NAME, "AAC");
        info->setDuration(aac_file.duration());
    }

    file.close();
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAACFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderAACFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(aac, DecoderAACFactory)
#endif
