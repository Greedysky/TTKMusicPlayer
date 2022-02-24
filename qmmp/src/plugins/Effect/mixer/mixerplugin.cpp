#include "mixerplugin.h"

#include <QSettings>

MixerPlugin *MixerPlugin::m_instance = nullptr;

MixerPlugin::MixerPlugin()
    : Effect()
{
    m_instance = this;
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_type = (MixerType)settings.value("ChannelMixer/type", MixerPlugin::Null).toInt();
}

MixerPlugin::~MixerPlugin()
{
    m_instance = nullptr;
    if(m_buffer)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

void MixerPlugin::applyEffect(Buffer *b)
{
    m_mutex.lock();
    switch(m_type)
    {
        case MonoToStereo:
        {
            if(!m_enabled)
            {
                break;
            }

            if(m_size < b->samples)
            {
                if(m_buffer)
                {
                    delete[] m_buffer;
                }
		
                m_size = b->samples;
                m_buffer = new float[b->samples];
            }

            memcpy(m_buffer, b->data, b->samples * sizeof(float));
            b->samples *= 2;

            if(b->samples > b->size)
            {
                delete[] b->data;
                b->size = b->samples;
                b->data = new float[b->size];
            }

            for(size_t i = 0; i < b->samples; ++i)
            {
                b->data[i] = m_buffer[i >> 1];
            }
            break;
        }
        case StereoToMono:
        {
            if(channels() != 2)
            {
                break;
            }

            float average = 0.0f;
            float* data = b->data;
            for(size_t i = 0; i < b->samples; i += 2)
            {
                average = (data[i] + data[i + 1]) / 2;
                average = qBound(-1.0f, average, 1.0f);
                data[i] = data[i + 1] = average;
            }
            break;
        }
        default: break;
    }
    m_mutex.unlock();
}

void MixerPlugin::configure(quint32 freq, ChannelMap map)
{
    switch(m_type)
    {
        case MonoToStereo:
        {
            m_enabled = (map.count() == 1);
            Effect::configure(freq, m_enabled ? ChannelMap(2) : map);
            break;
        }
        default: 
        {
            Effect::configure(freq, map);
            break;
        }
    }
}

void MixerPlugin::setType(MixerType type)
{
    m_mutex.lock();
    m_type = type;
    configure(sampleRate(), channelMap());
    m_mutex.unlock();
}

MixerPlugin* MixerPlugin::instance()
{
    return m_instance;
}
