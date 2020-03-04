#include <QFileInfo>
#include <qmmp/qmmp.h>
#include "alacwrap.h"

ALACWrap::ALACWrap(const QString &path)
{
    m_path = path;
    m_bitrate = 0;
    m_totalTime = 0;
    m_file = nullptr;
    alac = nullptr;
    stream = nullptr;
}

ALACWrap::~ALACWrap()
{
    close();
}

void ALACWrap::close()
{
    m_totalTime = 0;
    m_bitrate = 0;
    stream_destroy(stream);
}

bool ALACWrap::initialize()
{
    m_totalTime = 0.0;
    m_bitrate = 0;

    m_file = fopen(m_path.toLocal8Bit().data(), "rb");
    if(m_file == nullptr)
    {
        return false;
    }

    stream = stream_create_file(m_file, 1);
    if(!qtmovie_read(stream, &demux))
    {
        //ALAC: alac_decode: failed to load the QuickTime movie headers!
        return false;
    }

    alac = create_alac(demux.sample_size, demux.num_channels);
    alac_set_info(alac, (char *)demux.codecdata);

    unsigned long output_time = 0;
    unsigned int index = 0;
    unsigned int accum = 0;
    for( unsigned int i = 0; i < demux.num_sample_byte_sizes; i++)
    {
        unsigned int thissample_duration;
        while((demux.time_to_sample[index].sample_count + accum) <= i)
        {
            accum += demux.time_to_sample[index].sample_count;
            index++;
            if(index >= demux.num_time_to_samples)
            {
                //ALAC: get_song_length: sample %i does not have a duration
                return false;
            }
        }
        thissample_duration = demux.time_to_sample[index].sample_duration;
        output_time += thissample_duration;
    }

    m_totalTime= (output_time / demux.sample_rate) * 1000;
    outputBytes = 4096 * 4;
    decoded_frames = 0;
    m_bitrate = ((QFileInfo(m_path).size() * 1000.0) / m_totalTime) / 128;;

    return true;
}

qint64 ALACWrap::totalTime() const
{
    return m_totalTime;
}

int ALACWrap::bitrate() const
{
    return m_bitrate;
}

int ALACWrap::samplerate() const
{
    return demux.sample_rate;
}

int ALACWrap::channels() const
{
    return demux.num_channels;
}

qint64 ALACWrap::read(unsigned char *data, qint64 size)
{
    uint32_t iCopy;
    uint32_t iCurrSize = size;
    while (iCurrSize > 0)
    {
      if (decoded_frames == demux.num_sample_byte_sizes)
      {
          return 0;
      }

        stream_read(stream, demux.sample_byte_size[decoded_frames], stream_buffer);
        decode_frame(alac, stream_buffer, buffer, &outputBytes);
        decoded_frames = 0;

      if (iCurrSize > outputBytes-decoded_frames)
        iCopy = outputBytes-decoded_frames;
      else
        iCopy = iCurrSize;

      memcpy(data,buffer+decoded_frames,iCopy);
      data += iCopy;
      iCurrSize -= iCopy;
      decoded_frames += iCopy;
    }
    if (iCurrSize == -1)
      return -1;

    return size-iCurrSize;
}

void ALACWrap::seek(qint64 pos)
{
    stream_setpos(stream, pos);
}
