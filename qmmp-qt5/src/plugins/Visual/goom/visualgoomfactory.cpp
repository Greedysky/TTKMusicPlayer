#include <qmmp/qmmp.h>
#include "visualgoomfactory.h"
#include "goomwidget.h"

const VisualProperties VisualGoomFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Goom");
    properties.shortName = "goom";
    properties.hasSettings = false;
    properties.hasAbout = true;
    return properties;
}

Visual *VisualGoomFactory::create(QWidget *parent)
{
    return new GoomWidget(parent);
}
