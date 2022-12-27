#include "outputpulseaudiofactory.h"
#include "outputpulseaudio.h"

OutputProperties OutputPulseAudioFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("PulseAudio Plugin");
    properties.shortName = "pulse";
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

void OutputPulseAudioFactory::showSettings(QWidget* parent)
{
    Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(pulseaudio, OutputPulseAudioFactory)
#endif
