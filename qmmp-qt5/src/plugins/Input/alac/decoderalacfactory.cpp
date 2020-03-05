#include "decoderalacfactory.h"
#include "decoder_alac.h"
#include "alacwrap.h"

bool DecoderALACFactory::canDecode(QIODevice *) const
{
    return false;
}

DecoderProperties DecoderALACFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("ALAC Plugin");
    properties.filters << "*.alac";
    properties.description = tr("ALAC Files");
    properties.shortName = "alac";
    properties.hasSettings = false;
    properties.noInput = true;
    properties.protocols << "file";
    return properties;
}

Decoder *DecoderALACFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderALAC(path);
}

QList<TrackInfo *> DecoderALACFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    ALACWrap alac(path.toUtf8().constData());
    if(!alac.initialize())
    {
        return QList <TrackInfo *>();
    }

    TrackInfo *info = new TrackInfo(path);
    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, alac.bitrate());
        info->setValue(Qmmp::SAMPLERATE, alac.samplerate());
        info->setValue(Qmmp::CHANNELS, alac.channels());
        info->setDuration(alac.totalTime());
    }

    return QList <TrackInfo *>() << info;
}

MetaDataModel* DecoderALACFactory::createMetaDataModel(const QString &, bool)
{
    return nullptr;
}
