#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualfspekfactory.h"
#include "fspek.h"

const SpekProperties VisualFSpekFactory::properties() const
{
    SpekProperties properties;
    properties.name = tr("FSpek Plugin");
    properties.shortName = "fspek";
    properties.hasSettings = false;
    properties.hasAbout = false;
    return properties;
}

Spek *VisualFSpekFactory::create(QWidget *parent)
{
    return new FSpek(parent);
}

Q_EXPORT_PLUGIN2(fspek,VisualFSpekFactory)

