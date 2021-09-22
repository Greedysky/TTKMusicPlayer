#include "effectvoiceremovalfactory.h"
#include "voiceremovalplugin.h"

const EffectProperties EffectVoiceRemovalFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Voice Removal Plugin");
    properties.shortName = "voiceremoval";
    return properties;
}

Effect *EffectVoiceRemovalFactory::create()
{
    return new VoiceRemovalPlugin();
}

void EffectVoiceRemovalFactory::showSettings(QWidget *parent)
{
   Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(voiceremoval, EffectVoiceRemovalFactory)
#endif
