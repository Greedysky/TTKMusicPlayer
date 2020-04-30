#include "visualfloridgoomfactory.h"
#include "goomwidget.h"

VisualProperties VisualFloridGoomFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Goom Plugin");
    properties.shortName = "floridgoom";
    properties.hasSettings = false;
    return properties;
}

Visual *VisualFloridGoomFactory::create(QWidget *parent)
{
    return new GoomWidget(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridgoom, VisualFloridGoomFactory)
#endif
