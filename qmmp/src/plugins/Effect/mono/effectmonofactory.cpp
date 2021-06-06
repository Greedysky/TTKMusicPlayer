#include "effectmonofactory.h"
#include "monoplugin.h"

const EffectProperties EffectMonoFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Extra Mono Plugin");
    properties.shortName = "mono";
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

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(mono, EffectMonoFactory)
#endif
