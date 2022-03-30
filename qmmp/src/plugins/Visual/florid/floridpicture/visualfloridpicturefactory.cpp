#include "visualfloridpicturefactory.h"
#include "floridpicture.h"

VisualProperties VisualFloridPictureFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Florid Picture Plugin");
    properties.shortName = "floridpicture";
    return properties;
}

Visual *VisualFloridPictureFactory::create(QWidget *parent)
{
    return new FloridPicture(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(floridpicture, VisualFloridPictureFactory)
#endif
