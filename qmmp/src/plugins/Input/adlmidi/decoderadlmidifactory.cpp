#include "decoderadlmidifactory.h"
#include "decoder_adlmidi.h"
#include "adlmidihelper.h"

bool DecoderAdlMidiFactory::canDecode(QIODevice *input) const
{
    QFile *file = static_cast<QFile*>(input);
    if(!file)
        return false;

    AdlMidiHelper helper(file->fileName());
    return helper.initialize();
}

DecoderProperties DecoderAdlMidiFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("AdlMidi Plugin");
    properties.shortName = "adlmidi";
    properties.filters << "*.mid"<< "*.midi" << "*.mus" << "*.rmi" << "*.xmi";
    properties.description = "Midi File";
    properties.protocols << "file";
    properties.noInput = true;
    return properties;
}

Decoder *DecoderAdlMidiFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderAdlMidi(path);
}

QList<TrackInfo*> DecoderAdlMidiFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    AdlMidiHelper helper(path);
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
        info->setValue(Qmmp::FORMAT_NAME, "AdlMidi");
        info->setDuration(helper.totalTime());
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderAdlMidiFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderAdlMidiFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(adlmidi, DecoderAdlMidiFactory)
#endif
