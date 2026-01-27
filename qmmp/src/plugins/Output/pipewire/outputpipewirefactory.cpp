#include "outputpipewirefactory.h"
#include "outputpipewire.h"

OutputProperties OutputPipeWireFactory::properties() const
{
    OutputProperties properties;
    properties.name = tr("PipeWire Plugin");
    properties.shortName = "pipewire";
    return properties;
}

Output* OutputPipeWireFactory::create()
{
    return new OutputPipeWire();
}

Volume *OutputPipeWireFactory::createVolume()
{
    return new VolumePipeWire();
}

QDialog *OutputPipeWireFactory::createSettings(QWidget *parent)
{
    Q_UNUSED(parent);
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtPlugin>
Q_EXPORT_PLUGIN2(pipewire, OutputPipeWireFactory)
#endif
