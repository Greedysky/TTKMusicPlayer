#include "ladspahelper.h"
#include "ladspahost.h"

#include <QApplication>

LADSPAHelper::LADSPAHelper()
    : Effect()
{
    if(!LADSPAHost::instance())
    {
        new LADSPAHost(qApp);
    }
}

void LADSPAHelper::applyEffect(Buffer *b)
{
    LADSPAHost::instance()->applyEffect(b->data, b->samples);
}

void LADSPAHelper::configure(quint32 freq, ChannelMap map)
{
    LADSPAHost::instance()->configure(freq, map.count());
    Effect::configure(freq, map);
}
