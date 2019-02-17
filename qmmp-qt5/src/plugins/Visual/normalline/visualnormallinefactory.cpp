#include <qmmp/qmmp.h>
#include "visualnormallinefactory.h"
#include "normalline.h"

VisualProperties VisualNormalLineFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal Line Plugin");
    properties.shortName = "normalline";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualNormalLineFactory::create(QWidget *parent)
{
    return new NormalLine(parent);
}
