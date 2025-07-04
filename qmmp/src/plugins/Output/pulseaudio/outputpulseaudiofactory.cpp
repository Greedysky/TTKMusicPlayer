#include "outputpulseaudiofactory.h"
#include "outputpulseaudio.h"

OutputProperties OutputPulseAudioFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("PulseAudio Plugin");
    properties.shortName = "pulseaudio";
    return properties;
}

Output* OutputPulseAudioFactory::create()
{
    return new OutputPulseAudio();
}

Volume *OutputPulseAudioFactory::createVolume()
{
    return new VolumePulseAudio();
}

QDialog *OutputPulseAudioFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(pulseaudio, OutputPulseAudioFactory)
#endif
