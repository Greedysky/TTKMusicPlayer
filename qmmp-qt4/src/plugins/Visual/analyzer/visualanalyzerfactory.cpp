#include <QtPlugin>
#include <qmmp/qmmp.h>
#include "visualanalyzerfactory.h"
#include "analyzer.h"

const VisualProperties VisualAnalyzerFactory::properties() const
{
    VisualProperties properties;
    properties.name = tr("Analyzer Plugin");
    properties.shortName = "analyzer";
    properties.hasSettings = true;
    properties.hasAbout = false;
    return properties;
}

Visual *VisualAnalyzerFactory::create(QWidget *parent)
{
    return new Analyzer(parent);
}

Q_EXPORT_PLUGIN2(analyzer,VisualAnalyzerFactory)
