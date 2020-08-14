#include "channelconverter_p.h"

ChannelConverter::ChannelConverter(ChannelMap out_map)
    : m_out_map(out_map)
{

}

ChannelConverter::~ChannelConverter()
{
    if(m_tmp_buf)
    {
        delete [] m_tmp_buf;
        m_tmp_buf = nullptr;
    }
}

void ChannelConverter::configure(quint32 srate, ChannelMap in_map)
{
    Effect::configure(srate, m_out_map);

    if((m_disabled = (in_map == m_out_map)))
        return;

    if((m_disabled = (in_map.count() == 1 && m_out_map.count() == 1)))
        return;

    m_in_map = in_map;
    m_tmp_buf = new float[QMMP_BLOCK_FRAMES * in_map.count()];
    m_tmp_size = QMMP_BLOCK_FRAMES * in_map.count();

    QStringList reorderStringList;    
    for(int i = 0; i < m_out_map.count(); ++i)
    {
        m_reorder_array[i] = m_out_map.indexOf(in_map.at(i % in_map.count()));
        reorderStringList << QString("%1").arg(m_reorder_array[i]);
    }

    qDebug("ChannelConverter: {%s} ==> {%s}; {%s}", qPrintable(in_map.toString()),
           qPrintable(m_out_map.toString()), qPrintable(reorderStringList.join(",")));
}

void ChannelConverter::applyEffect(Buffer *b)
{
    if(m_disabled)
        return;

    int in_channels = m_in_map.count();
    int out_channels = m_out_map.count();

    if(b->samples > m_tmp_size)
    {
        delete [] m_tmp_buf;
        m_tmp_buf = new float[b->samples];
        m_tmp_size = b->samples;
    }
    memcpy(m_tmp_buf, b->data, b->samples * sizeof(float));

    size_t samples = b->samples * out_channels / in_channels;
    if(samples > b->size)
    {
        delete [] b->data;
        b->data = new float[samples];
        b->size = samples;
    }

    float *in = m_tmp_buf;
    float *out = b->data;

    for(unsigned long i = 0; i < b->samples / in_channels; ++i)
    {
        for(int j = 0; j < out_channels; ++j)
            out[j] = m_reorder_array[j] < 0 ? 0 : in[m_reorder_array[j]];
        in += in_channels;
        out += out_channels;
    }

    b->samples = samples;
}
