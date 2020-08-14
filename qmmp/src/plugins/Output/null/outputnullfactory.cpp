#include "outputnull.h"
#include "outputnullfactory.h"

OutputProperties OutputNullFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("Null Plugin");
    properties.hasSettings = false;
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

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(null, OutputNullFactory)
#endif
