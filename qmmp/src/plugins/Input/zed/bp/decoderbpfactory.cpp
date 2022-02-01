#include "decoderbpfactory.h"
#include "bphelper.h"
#include "decoder_bp.h"

bool DecoderBpFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderBpFactory::properties() const
{
    DecoderProperties properties;
    properties.name = "BP Plugin";
    properties.shortName = "bp";
    properties.filters << "*.bp" << "*.bp3";
    properties.description = "BP SoundMon Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderBpFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderBp(path);
}

QList<TrackInfo*> DecoderBpFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    BpHelper helper(path);
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
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "BP");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderBpFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderBpFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(bp, DecoderBpFactory)
#endif
