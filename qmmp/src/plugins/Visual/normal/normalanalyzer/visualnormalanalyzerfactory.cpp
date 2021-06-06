#include "visualnormalanalyzerfactory.h"
#include "normalanalyzer.h"

VisualProperties VisualNormalAnalyzerFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal Analyzer Plugin");
    properties.shortName = "normalanalyzer";
    return properties;
}

Visual *VisualNormalAnalyzerFactory::create(QWidget *parent)
{
    return new NormalAnalyzer(parent);
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(normalanalyzer, VisualNormalAnalyzerFactory)
#endif
