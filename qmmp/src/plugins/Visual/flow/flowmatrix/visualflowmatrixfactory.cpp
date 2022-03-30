#include "visualflowmatrixfactory.h"
#include "flowmatrix.h"

VisualProperties VisualFlowMatrixFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Flow Matrix Plugin");
    properties.shortName = "flowmatrix";
    return properties;
}

Visual *VisualFlowMatrixFactory::create(QWidget *parent)
{
    return new FlowMatrix(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(flowmatrix, VisualFlowMatrixFactory)
#endif
