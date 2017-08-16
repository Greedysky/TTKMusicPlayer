#include <qmmp/qmmp.h>
#include "visuallineplusfactory.h"
#include "lineplus.h"

const VisualProperties VisualLinePlusFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("LinePlus Plugin");
    properties.shortName = "lineplus";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualLinePlusFactory::create(QWidget *parent)
{
    return new LinePlus(parent);
}
