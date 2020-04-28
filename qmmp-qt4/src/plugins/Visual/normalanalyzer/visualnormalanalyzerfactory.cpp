#include <QtPlugin>
#include "visualnormalanalyzerfactory.h"
#include "normalanalyzer.h"

VisualProperties VisualNormalAnalyzerFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Normal Analyzer Plugin");
    properties.shortName = "normalanalyzer";
    properties.hasSettings = true;
    return properties;
}

Visual *VisualNormalAnalyzerFactory::create(QWidget *parent)
{
    return new NormalAnalyzer(parent);
}

Q_EXPORT_PLUGIN2(normalanalyzer,VisualNormalAnalyzerFactory)
