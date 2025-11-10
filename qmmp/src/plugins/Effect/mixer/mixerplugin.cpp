#include "mixerplugin.h"

#include <QSettings>

MixerPlugin *MixerPlugin::m_instance = nullptr;

MixerPlugin::MixerPlugin()
    : Effect()
{
    m_instance = this;

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_type = (MixerType)settings.value("ChannelMixer/type", MixerPlugin::Null).toInt();
}

MixerPlugin::~MixerPlugin()
{
    m_instance = nullptr;
    delete[] m_buffer;
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
                delete[] m_buffer;
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

            float* data = b->data;
            for(size_t i = 0; i < b->samples; i += 2)
            {
                const float frontLeft = data[i];
                const float frontRight = data[i + 1];
                data[i] = data[i + 1] = (frontLeft + frontRight) / 2;
            }
            break;
        }
        case StereoToQuadro:
        {
            if(!m_enabled)
            {
                break;
            }

            if(m_size < b->samples)
            {
                delete[] m_buffer;
                m_size = b->samples;
                m_buffer = new float[b->samples];
            }

            memcpy(m_buffer, b->data, b->samples * sizeof(float));
            b->samples *= 4;

            if(b->samples > b->size)
            {
                delete[] b->data;
                b->size = b->samples;
                b->data = new float[b->size];
            }

            for(size_t i = 0; i < b->samples; i += 4)
            {
                const size_t index = i >> 1;
                const float frontLeft = m_buffer[index];
                const float frontRight = m_buffer[index + 1];
                b->data[i] = b->data[i + 2] = frontLeft;
                b->data[i + 1] = b->data[i + 3] = frontRight;
            }
            break;
        }
        case QuadroToStereo:
        {
            if(channels() != 4)
            {
                break;
            }

            float* data = b->data;
            for(size_t i = 0; i < b->samples; i += 4)
            {
                const float frontLeft = data[i];
                const float frontRight = data[i + 1];
                const float backLeft = data[i + 2];
                const float backRight = data[i + 3];
                data[i] = data[i + 2] = frontLeft + backLeft * 0.7;
                data[i + 1] = data[i + 3] = frontRight + backRight * 0.7;
            }
            break;
        }
        case Quadro5ToStereo:
        {
            if(channels() != 5)
            {
                break;
            }

            float* data = b->data;
            for(size_t i = 0; i < b->samples; i += 5)
            {
                const float frontLeft = data[i];
                const float frontRight = data[i + 1];
                const float center = data[i + 2];
                const float rearLeft = data[i + 3];
                const float rearRight = data[i + 4];
                data[i] = data[i + 3] = frontLeft + center * 0.5 + rearLeft * 0.5;
                data[i + 1] = data[i + 4] = frontRight + center * 0.5 + rearRight * 0.5;
                data[i + 2] = center * 0.5;
            }
            break;
        }
        case SurroundToStereo:
        {
            if(channels() != 6)
            {
                break;
            }

            float* data = b->data;
            for(size_t i = 0; i < b->samples; i += 6)
            {
                const float frontLeft = data[i];
                const float frontRight = data[i + 1];
                const float center = data[i + 2];
                const float lfe = data[i + 3];
                const float rearLeft = data[i + 4];
                const float rearRight = data[i + 5];
                data[i] = data[i + 4] = frontLeft + center * 0.5 + lfe * 0.5 + rearLeft * 0.5;
                data[i + 1] = data[i + 5] = frontRight + center * 0.5 + lfe * 0.5 + rearRight * 0.5;
                data[i + 2] = center * 0.5;
                data[i + 3] = lfe * 0.5;
            }
            break;
        }
        default:
            break;
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
        case StereoToQuadro:
        {
            m_enabled = (map.count() == 2);
            Effect::configure(freq, m_enabled ? ChannelMap(4) : map);
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
