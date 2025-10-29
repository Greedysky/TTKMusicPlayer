#include "sonicplugin.h"

#include <cmath>
#include <QSettings>

SonicPlugin *SonicPlugin::m_instance = nullptr;

SonicPlugin::SonicPlugin()
    : Effect()
{
    m_instance = this;
}

SonicPlugin::~SonicPlugin()
{
    deinit();
}

void SonicPlugin::applyEffect(Buffer *b)
{
    sonicWriteFloatToStream(m_stream, b->data, b->samples);
    const size_t samples = sonicSamplesAvailable(m_stream);

    if(samples > b->size)
    {
        delete[] b->data;
        b->size = std::ceil(samples * 1.0 / b->size) * QMMP_BLOCK_FRAMES * channels();
        b->data = new float[b->size];
        b->samples = b->size;
    }

    int samplesWritten = 0;
    float buffer[b->size * 2];
    memset(buffer, 0, sizeof(float) * b->size * 2);

    do
    {
        samplesWritten = sonicReadFloatFromStream(m_stream, buffer, b->size);
        if(samplesWritten > 0)
        {
          memcpy(b->data, buffer, samplesWritten * sizeof(float));
          b->samples = samplesWritten;
      }
    } while(samplesWritten > 0);
}

void SonicPlugin::configure(quint32 srate, ChannelMap map)
{
    Effect::configure(srate, map);

    deinit();
    m_stream = sonicCreateStream(srate, map.count());
    sonicSetVolume(m_stream, 1);
    sonicSetQuality(m_stream, 0);

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    setRatio(settings.value("Sonic/ratio", DEFAULT_RATIO).toInt());
}

void SonicPlugin::setRatio(int ratio)
{
    sonicSetRate(m_stream, ratio * 1.0 / DEFAULT_RATIO);
}

SonicPlugin* SonicPlugin::instance()
{
    return m_instance;
}

void SonicPlugin::deinit()
{
    if(m_stream)
    {
        sonicDestroyStream(m_stream);
    }
}
