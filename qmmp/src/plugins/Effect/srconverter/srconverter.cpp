#include "srconverter.h"

#include <QSettings>

SRConverter::SRConverter()
    : Effect()
{
    int converter_type_array[] = {
        SRC_SINC_BEST_QUALITY,
        SRC_SINC_MEDIUM_QUALITY,
        SRC_SINC_FASTEST,
        SRC_ZERO_ORDER_HOLD,
        SRC_LINEAR
    };

    const QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    m_sampleRate = settings.value("SRC/sample_rate", 48000).toInt();
    m_engine = converter_type_array[settings.value("SRC/engine", 0).toInt()];
    memset(&m_data, 0, sizeof(SRC_DATA));
}

SRConverter::~SRConverter()
{
    deinit();
}

void SRConverter::applyEffect(Buffer *b)
{
    if(m_state && b->samples > 0)
    {
        m_data.end_of_input = 0;
        m_data.data_in = b->data;
        m_data.input_frames = b->samples / channels();
        m_data.output_frames = m_data.src_ratio * m_data.input_frames + 1;

        if(src_process(m_state, &m_data) > 0)
        {
            qWarning("SRConverter: src_process()");
        }

        b->samples = m_data.output_frames_gen * channels();
        m_data.data_in = nullptr;
        m_data.input_frames = 0;

        if(b->samples > b->size)
        {
            delete[] b->data;
            b->data = new float[b->samples];
            b->size = b->samples;
        }

        memcpy(b->data, m_data.data_out, b->samples * sizeof(float));
    }
}

void SRConverter::configure(quint32 freq, ChannelMap map)
{
    deinit();
    if(freq != m_sampleRate)
    {
        m_state = src_new(m_engine, map.count(), nullptr);
        if(m_state)
        {
            m_data.src_ratio = (float)m_sampleRate/(float)freq;
            src_set_ratio(m_state, m_data.src_ratio);
        }

        m_data.data_out = new float[int(m_data.src_ratio * QMMP_BLOCK_FRAMES * map.count() * 2 + 2)];
    }
    Effect::configure(m_sampleRate, map);
}

void SRConverter::deinit()
{
    if(m_state)
    {
        src_reset(m_state);
        src_delete(m_state);
    }
    m_state = nullptr;

    delete[] m_data.data_in;
    m_data.data_in = nullptr;

    delete[] m_data.data_out;
    m_data.data_out = nullptr;

    m_data.end_of_input = 0;
    m_data.input_frames = 0;
    m_data.output_frames = 0;
}
