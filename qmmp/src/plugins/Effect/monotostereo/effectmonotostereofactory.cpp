#include "effectmonotostereofactory.h"
#include "monotostereoplugin.h"

const EffectProperties EffectMonoToStereoFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Mono to Stereo Converter Plugin");
    properties.shortName = "monotostereo";
    return properties;
}

Effect *EffectMonoToStereoFactory::create()
{
    return new MonoToStereoPlugin();
}

void EffectMonoToStereoFactory::showSettings(QWidget *parent)
{
   Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(monotostereo, EffectMonoToStereoFactory)
#endif
