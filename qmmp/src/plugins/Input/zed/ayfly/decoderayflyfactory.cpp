#include "decoderayflyfactory.h"
#include "ayflyhelper.h"
#include "decoder_ayfly.h"

bool DecoderAyflyFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderAyflyFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AyFly Plugin");
    properties.shortName = "alfly";
    properties.filters << "*.vtx" << "*.asc" << "*.sqt" << "*.psg";
    properties.filters << "*.stc" << "*.stp";
    properties.filters << "*.pt1" << "*.pt2" << "*.pt3";
    properties.filters << "*.ay";
    properties.filters << "*.emul";
    properties.description = "AY/YM Audio File";
    properties.noInput = true;
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
        info->setValue(Qmmp::FORMAT_NAME, "AyFly");
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

void DecoderAyflyFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(alfly, DecoderAyflyFactory)
#endif
