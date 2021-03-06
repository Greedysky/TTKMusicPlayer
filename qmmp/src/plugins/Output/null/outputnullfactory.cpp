#include "outputnull.h"
#include "outputnullfactory.h"

OutputProperties OutputNullFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("Null Plugin");
    properties.shortName = "null";
    return properties;
}

Output* OutputNullFactory::create()
{
    return new OutputNull();
}

Volume *OutputNullFactory::createVolume()
{
    return nullptr;
}

void OutputNullFactory::showSettings(QWidget *parent)
{
    Q_UNUSED(parent);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(null, OutputNullFactory)
#endif
