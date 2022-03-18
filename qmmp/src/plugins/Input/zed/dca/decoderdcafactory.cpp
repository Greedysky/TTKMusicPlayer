#include "decoderdcafactory.h"
#include "dcahelper.h"
#include "decoder_dca.h"

bool DecoderDCAFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderDCAFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("DTS Plugin");
    properties.shortName = "dca";
    properties.filters << "*.dts" << "*.cpt";
    properties.description = "DTS Coherent Acoustics Audio File";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderDCAFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderDCA(path);
}

QList<TrackInfo*> DecoderDCAFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    DCAHelper helper(path);
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
        info->setValue(Qmmp::FORMAT_NAME, "DTS");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderDCAFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderDCAFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(dca, DecoderDCAFactory)
#endif
