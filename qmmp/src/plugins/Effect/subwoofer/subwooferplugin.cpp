#include "subwooferplugin.h"

#include <qmath.h>
#include <QSettings>

static BiquadSCoefs coefs[STAGE_LENGTH] = {
    {1.0, 1.931851652578, 1.0, 0.0, 0.0, 1.0},
    {1.0, 1.414213562373, 1.0, 0.0, 0.0, 1.0},
    {1.0, 0.517638090205, 1.0, 0.0, 0.0, 1.0}
};

static void biquadBilinearDesign(BiquadZCoefs *out, const BiquadSCoefs *in, float omega)
{
    const float t = 2.0 * tan(omega * M_PI / 2);
    const float v = (4.0 * in->a + 2.0 * in->b * t + in->c * t * t);

    out->a2 = (4.0 * in->a - 2.0 * in->b * t + in->c * t * t) / v;
    out->a1 = (2.0 * in->c * t * t - 8.0 * in->a) / v;
    out->a0 = 1.0;

    out->b2 = (4.0 * in->d - 2.0 * in->e * t + in->f * t * t) / v * in->c / in->f;
    out->b1 = (2.0 * in->f * t * t - 8.0 * in->d) / v * in->c / in->f;
    out->b0 = (4 * in->d + in->f * t * t + 2.0 * in->e * t) / v * in->c / in->f;
}

static void biquadReset(BiquadBuffer *buf)
{
    buf->x1 = buf->x2 = buf->y1 = buf->y2 = 0;
}

static float biquadProcess(BiquadBuffer *buf, const BiquadZCoefs *in, float x)
{
    const float center = x * in->b0 + in->b1 * buf->x1 + in->b2 * buf->x2;
    const float y = in->a0 * center - in->a1 * buf->y1 - in->a2 * buf->y2;

    buf->x2 = buf->x1;
    buf->x1 = x;

    buf->y2 = buf->y1;
    buf->y1 = y;
    return y;
}

static void butterworth6Design(BiquadZCoefs *out, int samplerate, int cutout)
{
    const float omega = cutout * 1.0 / samplerate;
    for(int i = 0; i < STAGE_LENGTH; ++i)
    {
        biquadBilinearDesign(out + i, coefs + i, omega);
    }
}

static float butterworth6Process(BiquadBuffer *buf, const BiquadZCoefs *in, float x)
{
    float y = biquadProcess(&buf[0], &in[0], x);
    for(int i = 1; i < STAGE_LENGTH; ++i)
    {
        y = biquadProcess(&buf[i], &in[i], y);
    }
    return y;
}

static float shaitan(float x)
{
    return 2.5 * atan(0.9 * x) + 2.5 * sqrt(1 - pow(0.9 * x,  2)) - 2.5;
}


SubwooferPlugin *SubwooferPlugin::m_instance = nullptr;

SubwooferPlugin::SubwooferPlugin()
    : Effect()
{
    m_instance = this;
    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    setLevel(settings.value("Subwoofer/level", 10).toUInt());
    m_cutoff = settings.value("Subwoofer/cutoff", 250).toUInt();
}

SubwooferPlugin::~SubwooferPlugin()
{
    m_instance = nullptr;
}

void SubwooferPlugin::applyEffect(Buffer *b)
{
    if(channels() != 2)
    {
        return;
    }

    m_mutex.lock();
    const float level = m_level / 10.f;

    float *data = b->data;
    for(size_t i = 0; i < b->samples; i += 2)
    {
        const float x0 = data[i];
        const float x1 = data[i + 1];
        const float lf0 = butterworth6Process(m_buffer[0], m_coefs, x0);
        const float lf1 = butterworth6Process(m_buffer[1], m_coefs, x1);
        const float lfs = shaitan((lf0 + lf1) / 2.0 * level) / level;

        data[i] = x0 - lf0 + lfs;
        data[i + 1] = x1 - lf1 + lfs;
    }
    m_mutex.unlock();
}

void SubwooferPlugin::configure(quint32 freq, ChannelMap map)
{
    if(sampleRate() != freq)
    {
        Effect::configure(freq, map);
        reset();
    }
}

void SubwooferPlugin::reset()
{
    for(int i = 0; i < 2; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            biquadReset(&m_buffer[i][j]);
        }
    }

    butterworth6Design(m_coefs, sampleRate(), m_cutoff);
}

void SubwooferPlugin::setLevel(float level)
{
    m_mutex.lock();
    m_level = level;
    m_mutex.unlock();
}

void SubwooferPlugin::setCutOff(int value)
{
    m_mutex.lock();
    m_cutoff = value;
    reset();
    m_mutex.unlock();
}

SubwooferPlugin* SubwooferPlugin::instance()
{
    return m_instance;
}
