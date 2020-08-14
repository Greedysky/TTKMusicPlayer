#include "volume.h"

bool Volume::isMuted() const
{
    return m_mutedInternal;
}

void Volume::setMuted(bool mute)
{
    m_mutedInternal = mute;
}

Volume::VolumeFlags Volume::flags() const
{
    return Volume::VolumeFlags();
}
