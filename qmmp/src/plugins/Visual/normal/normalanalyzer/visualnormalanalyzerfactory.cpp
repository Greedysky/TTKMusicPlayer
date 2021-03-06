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

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(normalanalyzer, VisualNormalAnalyzerFactory)
#endif
