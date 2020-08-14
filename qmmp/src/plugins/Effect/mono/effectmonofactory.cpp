#include "effectmonofactory.h"
#include "monoplugin.h"

const EffectProperties EffectMonoFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Extra Mono Plugin");
    properties.shortName = "mono";
    properties.hasSettings = false;
    return properties;
}

Effect *EffectMonoFactory::create()
{
    return new MonoPlugin();
}

void EffectMonoFactory::showSettings(QWidget *parent)
{
   Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(mono, EffectMonoFactory)
#endif
