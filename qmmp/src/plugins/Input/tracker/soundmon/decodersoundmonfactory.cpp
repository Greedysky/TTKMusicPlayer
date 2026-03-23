#include "decodersoundmonfactory.h"
#include "soundmonhelper.h"
#include "decoder_soundmon.h"

bool DecoderSoundMonFactory::canDecode(QIODevice *input) const
{
    const QFile * const file = qobject_cast<QFile*>(input);
    if(!file)
    {
        return false;
    }

    SoundMonHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderSoundMonFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("BP SoundMon Plugin");
    properties.shortName = "soundmon";
    properties.filters << "*.bp" << "*.bp3";
    properties.description = "BP SoundMon Audio File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderSoundMonFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderSoundMon(path);
}

TrackInfoList DecoderSoundMonFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo raw(path), *info = &raw;
    if(parts == TrackInfo::Parts())
    {
        return {raw};
    }

    SoundMonHelper helper(path);
    if(!helper.initialize())
    {
        return {};
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, helper.bitrate());
        info->setValue(Qmmp::SAMPLERATE, helper.sampleRate());
        info->setValue(Qmmp::CHANNELS, helper.channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, helper.depth());
        info->setValue(Qmmp::FORMAT_NAME, "BP SoundMon");
        info->setDuration(helper.totalTime());
    }

    return {raw};
}

MetaDataModel* DecoderSoundMonFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

QDialog *DecoderSoundMonFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(soundmon, DecoderSoundMonFactory)
#endif
