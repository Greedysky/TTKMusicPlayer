#include "soxresampler.h"

#include <QSettings>
#include <math.h>

SoXResampler::SoXResampler()
    : Effect()
{
    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_sampleRate = settings.value("SOXR/sample_rate", 48000).toInt();
    m_quality = soxr_quality_spec(settings.value("SOXR/quality", SOXR_HQ).toInt(), 0);
}

SoXResampler::~SoXResampler()
{
    deinit();
}

void SoXResampler::applyEffect(Buffer *b)
{
    if(m_soxr && b->samples > 0)
    {
        size_t done = 0;
        soxr_process(m_soxr, b->data, b->samples / channels(), nullptr,
                     m_out, m_samples / channels(), &done);

        b->samples = done * channels();
        if(b->samples > b->size)
        {
            delete[] b->data;
            b->data = new float[b->samples];
            b->size = b->samples;
        }

        memcpy(b->data, m_out, b->samples * sizeof(float));
    }
}

void SoXResampler::configure(quint32 freq, ChannelMap map)
{
    deinit();
    if(freq != m_sampleRate)
    {
        soxr_error_t error = nullptr;
        m_soxr = soxr_create(freq, m_sampleRate, map.count(), &error, nullptr, &m_quality, nullptr);
        double ratio = (double)m_sampleRate / freq;
        m_samples = ratio * QMMP_BLOCK_FRAMES * map.count() * 2 + 2;
        m_out = new float[m_samples];
    }
    Effect::configure(m_sampleRate, map);
}

void SoXResampler::deinit()
{
    if(m_soxr)
    {
        soxr_delete(m_soxr);
        m_soxr = nullptr;
    }

    delete[] m_out;
    m_out = nullptr;
    m_samples = 0;
}
