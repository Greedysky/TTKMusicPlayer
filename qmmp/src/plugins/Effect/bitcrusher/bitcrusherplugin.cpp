#include "bitcrusherplugin.h"

#include <cmath>
#include <QSettings>

BitcrusherPlugin *BitcrusherPlugin::m_instance = nullptr;

BitcrusherPlugin::BitcrusherPlugin()
    : Effect()
{
    m_instance = this;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_depth = settings.value("Bitcrusher/depth", 32).toDouble();
    m_downsample = settings.value("Bitcrusher/downsample", 1.0).toDouble();
}

BitcrusherPlugin::~BitcrusherPlugin()
{
    m_instance = nullptr;
    delete[] m_buffer;
}

void BitcrusherPlugin::applyEffect(Buffer *b)
{
    m_mutex.lock();
    const double scale = std::pow(2.0, m_depth) / 2.;
    const double gain = (33.0 - m_depth) / 8.;

    float *data = b->data;
    for(size_t i = 0; i < b->samples; )
    {
        m_accumulator += m_downsample;
        for(int channel = 0; channel < channels(); ++channel)
        {
            if(m_accumulator >= 1.0)
            {
                m_buffer[channel] = std::floor((data[i] * gain) * scale + 0.5) / scale / gain;
            }

            data[i++] = m_buffer[channel];
        }

        if(m_accumulator >= 1.0)
        {
            m_accumulator -= 1.0;
        }
    }
    m_mutex.unlock();
}

void BitcrusherPlugin::configure(quint32 freq, ChannelMap map)
{
    if(channels() != map.count())
    {
        Effect::configure(freq, map);

        delete[] m_buffer;
        m_buffer = new float[channels()]{0};
    }

    memset(m_buffer, 0, sizeof(float));
}

void BitcrusherPlugin::setDepth(double depth)
{
    m_mutex.lock();
    m_depth = depth;
    m_mutex.unlock();
}

void BitcrusherPlugin::setDownsample(double downsample)
{
    m_mutex.lock();
    m_downsample = downsample;
    m_mutex.unlock();
}

BitcrusherPlugin* BitcrusherPlugin::instance()
{
    return m_instance;
}
