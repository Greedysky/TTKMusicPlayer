#include "volume.h"

class VolumePrivate
{
public:
    bool mutedInternal = false;

};


Volume::Volume()
    : d(new VolumePrivate)
{

}

Volume::~Volume()
{
    delete d;
}

bool Volume::isMuted() const
{
    return d->mutedInternal;
}

void Volume::setMuted(bool mute)
{
    d->mutedInternal = mute;
}

Volume::VolumeFlags Volume::flags() const
{
    return Volume::VolumeFlags();
}
