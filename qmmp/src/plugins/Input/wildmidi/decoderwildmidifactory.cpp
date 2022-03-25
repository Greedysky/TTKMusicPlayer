#include "decoderwildmidifactory.h"
#include "wildmidihelper.h"
#include "decoder_wildmidi.h"
#include "settingsdialog.h"

DecoderWildMidiFactory::DecoderWildMidiFactory()
{
    new WildMidiHelper(qApp);
}

bool DecoderWildMidiFactory::canDecode(QIODevice *input) const
{
    char buf[4];
    if(input->peek(buf, 4) != 4)
        return false;
#if defined(LIBWILDMIDI_VERSION) && (LIBWILDMIDI_VERSION >= 0x000400)
    return !memcmp(buf, "MThd", 4) || !memcmp(buf, "MUS", 3) || !memcmp(buf, "FORM", 4);
#else
    return !memcmp(buf, "MThd", 4);
#endif
}

DecoderProperties DecoderWildMidiFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("WildMidi Plugin");
    properties.shortName = "wildmidi";
    properties.filters << "*.mid";
#if defined(LIBWILDMIDI_VERSION) && (LIBWILDMIDI_VERSION >= 0x000400)
    properties.filters << "*.mus" << "*.xmi";
#endif
    properties.description = "Midi File";
    properties.protocols << "file";
    properties.hasSettings = true;
    properties.noInput = true;
    return properties;
}

Decoder *DecoderWildMidiFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(input);
    return new DecoderWildMidi(path);
}

QList<TrackInfo*> DecoderWildMidiFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);
    WildMidiHelper *helper = WildMidiHelper::instance();

    if((parts & TrackInfo::Properties) && helper->initialize() && helper->sampleRate())
    {
        void *midi_ptr = WildMidi_Open(QmmpPrintable(path));
        if(midi_ptr)
        {
            WildMidiHelper::instance()->addPtr(midi_ptr);
            _WM_Info *wm_info = WildMidi_GetInfo(midi_ptr);

            info->setValue(Qmmp::SAMPLERATE, helper->sampleRate());
            info->setValue(Qmmp::BITRATE, helper->bitrate());
            info->setValue(Qmmp::CHANNELS, helper->channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, helper->bitsPerSample());
            info->setValue(Qmmp::FORMAT_NAME, "WildMidi");
            info->setDuration((qint64)wm_info->approx_total_samples * 1000 / helper->sampleRate());

            WildMidi_Close(midi_ptr);
            WildMidiHelper::instance()->removePtr(midi_ptr);
        }
    }
    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderWildMidiFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    Q_UNUSED(path);
    Q_UNUSED(readOnly);
    return nullptr;
}

void DecoderWildMidiFactory::showSettings(QWidget *parent)
{
    (new SettingsDialog(parent))->show();
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wildmidi, DecoderWildMidiFactory)
#endif
