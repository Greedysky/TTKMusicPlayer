#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualgoomfactory.h"
#include "goomwidget.h"

const VisualProperties VisualGoomFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flash Goom Plugin");
    properties.shortName = "flashgoom";
    properties.hasSettings = false;
    properties.hasAbout = true;
    return properties;
}

Visual *VisualGoomFactory::create(QWidget *parent)
{
    return new GoomWidget(parent);
}

Q_EXPORT_PLUGIN2(flashgoom,VisualGoomFactory)
