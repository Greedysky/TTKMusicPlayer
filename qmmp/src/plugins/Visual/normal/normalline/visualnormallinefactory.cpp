#include "visualnormallinefactory.h"
#include "normalline.h"

VisualProperties VisualNormalLineFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal Line Plugin");
    properties.shortName = "normalline";
    return properties;
}

Visual *VisualNormalLineFactory::create(QWidget *parent)
{
    return new NormalLine(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(normalline, VisualNormalLineFactory)
#endif
