#include "outputwasapi.h"
#include "outputwasapifactory.h"

OutputProperties OutputWASAPIFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("WASAPI Plugin");
    properties.hasSettings = true;
    properties.shortName = "wasapi";
    return properties;
}

Output* OutputWASAPIFactory::create()
{
    return new OutputWASAPI();
}

Volume *OutputWASAPIFactory::createVolume()
{
    return new VolumeWASAPI;
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(wasapi, OutputWASAPIFactory)
#endif
