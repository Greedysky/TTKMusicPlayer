#include "silenceplugin.h"

#include <QSettings>
#include <qmath.h>

static float* alignFrame(int channels, float* begin, float* sample, bool end)
{
    if(!sample)
    {
        return nullptr;
    }

    int offset = sample - begin;
    if(end)
    {
        offset += channels;
    }
    return begin + (offset - offset % channels);
}

SilencePlugin *SilencePlugin::m_instance = nullptr;

SilencePlugin::SilencePlugin()
    : Effect()
{
    m_instance = this;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_threshold = settings.value("Silence/threshold", -40).toDouble();
}

SilencePlugin::~SilencePlugin()
{
    m_instance = nullptr;
}

void SilencePlugin::applyEffect(Buffer *b)
{
    m_mutex.lock();
    const float threshold = std::pow(10, m_threshold / 20.0f);

    float* first = nullptr;
    float* last = nullptr;

    float* data = b->data;
    for(size_t i = 0; i < b->samples; ++i)
    {
      if(data[i] > threshold || data[i] < -threshold)
      {
        if(!first)
        {
          first = &data[i];
        }

        last = &data[i];
      }
    }

    first = alignFrame(channels(), data, first, false);
    last = alignFrame(channels(), data, last, true);
    m_size = last - first;

    if(m_size < b->samples)
    {
        b->samples = b->size = m_size;
        if(first)
        {
            memcpy(b->data, first, m_size * sizeof(float));
        }
    }
    m_mutex.unlock();
}

void SilencePlugin::setThreshold(double value)
{
    m_mutex.lock();
    m_threshold = value;
    m_mutex.unlock();
}

SilencePlugin* SilencePlugin::instance()
{
    return m_instance;
}
