#include "decoder_mpc.h"

#include <cmath>

static mpc_int32_t mpc_callback_read(mpc_reader *reader, void *buffer, mpc_int32_t size)
{
    DecoderMPC *dmpc = static_cast<DecoderMPC*>(reader->data);
    return dmpc->input()->read((char *)buffer, size);
}

static mpc_bool_t mpc_callback_seek(mpc_reader *reader, mpc_int32_t offset)
{
    DecoderMPC *dmpc = static_cast<DecoderMPC*>(reader->data);
    return dmpc->input()->seek(offset);
}

static mpc_int32_t mpc_callback_tell(mpc_reader *reader)
{
    DecoderMPC *dmpc = static_cast<DecoderMPC*>(reader->data);
    return dmpc->input()->pos();
}

static mpc_bool_t  mpc_callback_canseek(mpc_reader *reader)
{
    DecoderMPC *dmpc = static_cast<DecoderMPC*>(reader->data);
    return !dmpc->input()->isSequential();
}

static mpc_int32_t mpc_callback_get_size(mpc_reader *reader)
{
    DecoderMPC *dmpc = static_cast<DecoderMPC*>(reader->data);
    return dmpc->input()->size();
}


DecoderMPC::DecoderMPC(QIODevice *input)
    : Decoder(input)
{

}

DecoderMPC::~DecoderMPC()
{
    m_len = 0;
    if(m_data)
    {
        if(m_data->demuxer)
            mpc_demux_exit(m_data->demuxer);
        delete m_data;
        m_data = nullptr;
    }
}

bool DecoderMPC::initialize()
{
    m_bitrate = 0;
    m_totalTime = 0;

    if(!input())
    {
        qWarning("DecoderMPC: cannot initialize.  No input.");
        return false;
    }

    if(!m_data)
    {
        m_data = new mpc_data;
    }

    m_data->reader.read = mpc_callback_read;
    m_data->reader.seek = mpc_callback_seek;
    m_data->reader.tell = mpc_callback_tell;
    m_data->reader.canseek = mpc_callback_canseek;
    m_data->reader.get_size = mpc_callback_get_size;
    m_data->reader.data = this;

    m_data->demuxer = mpc_demux_init(&m_data->reader);

    if(!m_data->demuxer)
        return false;
    mpc_demux_get_info(m_data->demuxer, &m_data->info);

    configure(data()->info.sample_freq, data()->info.channels, Qmmp::PCM_FLOAT);

    QMap<Qmmp::ReplayGainKey, double> rg_info; //replay gain information
    rg_info[Qmmp::REPLAYGAIN_ALBUM_GAIN] = data()->info.gain_album / 256.0;
    rg_info[Qmmp::REPLAYGAIN_TRACK_GAIN] = data()->info.gain_title / 256.0;
    rg_info[Qmmp::REPLAYGAIN_ALBUM_PEAK] = std::pow(10, data()->info.peak_album / 256.0 / 20.0);
    rg_info[Qmmp::REPLAYGAIN_TRACK_PEAK] = std::pow(10, data()->info.peak_title / 256.0 / 20.0);
    setReplayGainInfo(rg_info);

    m_totalTime = mpc_streaminfo_get_length(&data()->info) * 1000;
    qDebug("DecoderMPC: initialize success");
    return true;
}

qint64 DecoderMPC::totalTime() const
{
    return m_totalTime;
}

int DecoderMPC::bitrate() const
{
    return m_bitrate;
}

qint64 DecoderMPC::read(unsigned char *data, qint64 maxSize)
{
    mpc_frame_info frame;
    mpc_status err;
    MPC_SAMPLE_FORMAT buffer[MPC_DECODER_BUFFER_LENGTH];
    frame.buffer = (MPC_SAMPLE_FORMAT *) &buffer;
    m_len = 0;
    while(!m_len)
    {
        err = mpc_demux_decode(m_data->demuxer, &frame);
        if(err != MPC_STATUS_OK || frame.bits == -1)
        {
            m_len = 0;
            qDebug("finished");
            return 0;
        }
        else
        {
            m_len = frame.samples * this->data()->info.channels;
            memcpy(data, buffer, qMin(qint64(m_len * sizeof(float)), maxSize));
        }
    }
    m_bitrate = frame.bits * this->data()->info.sample_freq / 1152000;
    return m_len * sizeof(float);
}

void DecoderMPC::seek(qint64 time)
{
    mpc_demux_seek_second(data()->demuxer, (double)time / 1000);
}
