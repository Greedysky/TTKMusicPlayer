#include "decoderayflyfactory.h"
#include "ayflyhelper.h"
#include "decoder_ayfly.h"

bool DecoderAyflyFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    AyflyHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderAyflyFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AyFly Plugin");
    properties.shortName = "alfly";
    properties.filters << "*.asc" << "*.ay";
    properties.filters << "*.emul";
    properties.filters << "*.sqt";
    properties.filters << "*.st13" << "*.stc" << "*.stp" << "*.stp2";
    properties.filters << "*.psc" << "*.psg";
    properties.filters << "*.pt1" << "*.pt2" << "*.pt3";
    properties.filters << "*.vtx";
    properties.filters << "*.zxs";
    properties.description = "AY/YM Audio File";
    properties.protocols << "file";
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
        info->setValue(Qmmp::TITLE, helper.title());
        info->setValue(Qmmp::ARTIST, helper.author());
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "AY/YM");
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

QDialog *DecoderAyflyFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(alfly, DecoderAyflyFactory)
#endif
