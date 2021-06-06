#include "lightenvelopefactory.h"
#include "lightenvelope.h"

const LightProperties LightEnvelopeFactory::properties() const
{
    LightProperties properties;
    properties.name = tr("Light Envelope Plugin");
    properties.shortName = "lightenvelope";
    return properties;
}

Light *LightEnvelopeFactory::create(QWidget *parent)
{
    return new LightEnvelope(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(lightenvelope, LightEnvelopeFactory)
#endif
