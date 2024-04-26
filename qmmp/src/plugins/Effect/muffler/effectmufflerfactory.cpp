#include "effectmufflerfactory.h"
#include "mufflerplugin.h"

EffectProperties EffectMufflerFactory::properties() const
{
    EffectProperties properties;
    properties.name = tr("Voice Removal Plugin");
    properties.shortName = "muffler";
    return properties;
}

Effect *EffectMufflerFactory::create()
{
    return new MufflerPlugin();
}

void EffectMufflerFactory::showSettings(QWidget *parent)
{
   Q_UNUSED(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(muffler, EffectMufflerFactory)
#endif
