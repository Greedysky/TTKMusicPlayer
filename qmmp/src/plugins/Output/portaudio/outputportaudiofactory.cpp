#include "outputportaudiofactory.h"
#include "outputportaudio.h"
#include "settingsdialog.h"

OutputProperties OutputPortAudioFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("PortAudio Plugin");
    properties.shortName = "portaudio";
    properties.hasSettings = true;
    return properties;
}

Output* OutputPortAudioFactory::create()
{
    return new OutputPortAudio();
}

Volume *OutputPortAudioFactory::createVolume()
{
    return nullptr;
}

QDialog *OutputPortAudioFactory::createSettings(QWidget *parent)
{
    return new SettingsDialog(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(portaudio, OutputPortAudioFactory)
#endif
