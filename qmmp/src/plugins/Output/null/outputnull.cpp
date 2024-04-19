#include <unistd.h>
#include "outputnull.h"

OutputNull::OutputNull()
    : Output()
{

}

bool OutputNull::initialize(quint32 freq, ChannelMap map, Qmmp::AudioFormat format)
{
    switch(format)
    {
    case Qmmp::PCM_S8:
        m_bytes_per_second = freq * map.count();
        break;
    case Qmmp::PCM_S24LE:
    case Qmmp::PCM_S32LE:
         m_bytes_per_second = freq * map.count() * 4;
        break;
    case Qmmp::PCM_S16LE:
    default:
         m_bytes_per_second = freq * map.count() * 2;
    }
    configure(freq, map, format);
    return true;
}

qint64 OutputNull::latency()
{
    return 0;
}

qint64 OutputNull::writeAudio(unsigned char *data, qint64 maxSize)
{
   Q_UNUSED(data);
   usleep(maxSize * 1000000 / m_bytes_per_second);
   return maxSize;
}

void OutputNull::drain()
{

}

void OutputNull::reset()
{

}
