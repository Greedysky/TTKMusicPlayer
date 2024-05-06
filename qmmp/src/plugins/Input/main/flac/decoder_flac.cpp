#include "decoder_flac.h"

#include <taglib/fileref.h>
#include <taglib/flacfile.h>
#include <taglib/xiphcomment.h>
#include <taglib/id3v2header.h>
#include <taglib/tfilestream.h>
#include <taglib/id3v2framefactory.h>
#include <qmmp/cueparser.h>

#define BITRATE_CALC_TIME_MS 2000

static size_t pack_pcm_signed(FLAC__byte *output,
                              const FLAC__int32 * const input[],
                              unsigned samples,
                              unsigned channels, unsigned bps)
{
    unsigned channel = 0;

    uint8_t *data8 = (uint8_t *) output;
    uint16_t *data16 = (uint16_t *) output;
    uint32_t *data32 = (uint32_t *) output;

     for(unsigned sample = 0; sample < samples; ++sample)
     {
         for(channel = 0; channel < channels; ++channel)
         {
             switch(bps)
             {
             case 8:
                 *data8 = input[channel][sample] & 0xff;
                 data8++;
                 break;
             case 16:
                 *data16 = input[channel][sample] & 0xffffff;
                 data16++;
                 break;
             case 24:
                 *data32 = (input[channel][sample] << 8) & 0xffffff00;
                 data32++;
                 break;
             case 32:
                 *data32 = input[channel][sample];
                 data32++;
                 break;
             }
         }
     }

     if(bps == 24) // we encode to 32-bit words
         bps = 32;

     return samples * channels * bps / 8;
}

static int flac_decode(void *client_data, unsigned char *buf, int buf_len)
{
    flac_data *data = static_cast<flac_data *>(client_data);
    unsigned to_copy;

    if(!data->sample_buffer_fill)
    {

        if(FLAC__stream_decoder_get_state(data->decoder)
                == FLAC__STREAM_DECODER_END_OF_STREAM)
        {
            return 0;
        }

        if(!FLAC__stream_decoder_process_single(
                    data->decoder))
        {
            return 0;
        }
    }

    to_copy = qMin((unsigned)buf_len, data->sample_buffer_fill);
    memcpy(buf, data->sample_buffer, to_copy);
    memmove(data->sample_buffer,
             data->sample_buffer + to_copy,
             data->sample_buffer_fill - to_copy);
    data->sample_buffer_fill -= to_copy;
    return to_copy;
}


static FLAC__StreamDecoderReadStatus flac_callback_read(const FLAC__StreamDecoder*,
        FLAC__byte buffer[],
        size_t *bytes,
        void *client_data)
{
    flac_data *data = static_cast<flac_data *>(client_data);
    qint64 res = data->input->read((char *)buffer, *bytes);

    if(res > 0)
    {
        *bytes = res;
        data->read_bytes += res;
        return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
    }
    if(res == 0)
    {
        *bytes = res;
        return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
    }

    return FLAC__STREAM_DECODER_READ_STATUS_ABORT;

}

static FLAC__StreamDecoderWriteStatus flac_callback_write(const FLAC__StreamDecoder *d,
        const FLAC__Frame *frame,
        const FLAC__int32 *const buffer[],
        void *client_data)
{
    flac_data *data = static_cast<flac_data *>(client_data);
    const unsigned wide_samples = frame->header.blocksize;

    if(data->abort)
        return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;

    //bitrate calculation
    FLAC__uint64 decode_position = 0;
    if(FLAC__stream_decoder_get_decode_position(d, &decode_position))
    {
        if(decode_position > data->last_decode_position)
        {
            data->bitrate = (decode_position - data->last_decode_position) * 8 * frame->header.sample_rate /
                    frame->header.blocksize / 1000;
        }

        data->last_decode_position = decode_position;
    }
    else
    {
        data->frame_counter += wide_samples;
        if(data->frame_counter * 1000 / frame->header.sample_rate > BITRATE_CALC_TIME_MS)
        {
            data->bitrate = data->read_bytes * 8 * frame->header.sample_rate / data->frame_counter / 1000;
            data->frame_counter = 0;
            data->read_bytes = 0;
        }
    }

    data->sample_buffer_fill = pack_pcm_signed(
                                            data->sample_buffer,
                                            buffer, wide_samples,
                                            data->channels,
                                            data->bits_per_sample);

    return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

static FLAC__StreamDecoderTellStatus flac_callback_tell(const FLAC__StreamDecoder *,
        FLAC__uint64 *offset,
        void *client_data)
{
    flac_data *data = static_cast<flac_data *>(client_data);
    if(data->input->isSequential())
        return FLAC__STREAM_DECODER_TELL_STATUS_UNSUPPORTED;

    *offset = data->input->pos();
    return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

static FLAC__StreamDecoderSeekStatus flac_callback_seek(const FLAC__StreamDecoder *,
        FLAC__uint64 offset,
        void *client_data)
{
    flac_data *data = static_cast<flac_data *>(client_data);
    if(data->input->isSequential())
        return FLAC__STREAM_DECODER_SEEK_STATUS_UNSUPPORTED;

    return data->input->seek(offset)
           ? FLAC__STREAM_DECODER_SEEK_STATUS_OK
           : FLAC__STREAM_DECODER_SEEK_STATUS_ERROR;
}

static FLAC__StreamDecoderLengthStatus flac_callback_length(const FLAC__StreamDecoder *,
        FLAC__uint64 *stream_length,
        void *client_data)
{
    flac_data *data = static_cast<flac_data *>(client_data);
    if(data->input->isSequential())
        return FLAC__STREAM_DECODER_LENGTH_STATUS_UNSUPPORTED;

    *stream_length = data->input->size();
    return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

static void flac_callback_metadata(const FLAC__StreamDecoder *,
                                   const FLAC__StreamMetadata *metadata,
                                   void *client_data)
{
    flac_data *data = static_cast<flac_data *>(client_data);
    if(metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
    {
        qDebug("DecoderFLAC: getting metadata info");

        data->total_samples =
            (unsigned)(metadata->data.stream_info.total_samples
                       & 0xffffffff);
        data->bits_per_sample =
            metadata->data.stream_info.bits_per_sample;
        data->channels = metadata->data.stream_info.channels;
        data->sample_rate = metadata->data.stream_info.sample_rate;
        data->length = data->total_samples * 1000 / data->sample_rate;

        if(metadata->data.stream_info.total_samples > 0 && data->length > 0)
        {
            data->bitrate = data->input->size() * 8 /  data->length + 1.0f;
        }
        else
        {
            data->bitrate = 0;
        }
    }
}

static FLAC__bool flac_callback_eof(const FLAC__StreamDecoder *, void *)
{
    return false;
}

static void flac_callback_error(const FLAC__StreamDecoder *,
                                 FLAC__StreamDecoderErrorStatus status,
                                 void *)
{
    Q_UNUSED(status);
}


DecoderFLAC::DecoderFLAC(const QString &path, QIODevice *input)
    : Decoder(input),
      m_path(path)
{
    m_data = new flac_data;
    m_data->input = input;
}

DecoderFLAC::~DecoderFLAC()
{
    deinit();
    if(m_data)
    {
        if(m_data->decoder)
            FLAC__stream_decoder_delete(m_data->decoder);
        delete m_data;
        m_data = nullptr;
    }

    if(m_buf)
        delete[] m_buf;
    m_buf = nullptr;
}

bool DecoderFLAC::initialize()
{
    if(!m_data->input)
    {
        if(m_path.startsWith("flac://")) //embeded cue track
        {
            const QString &filePath = TrackInfo::pathFromUrl(m_path);

            TagLib::FileStream stream(QStringToFileName(filePath), true);
#if TAGLIB_MAJOR_VERSION >= 2
            TagLib::FLAC::File fileRef(&stream);
#else
            TagLib::FLAC::File fileRef(&stream, TagLib::ID3v2::FrameFactory::instance());
#endif
            //looking for cuesheet comment
            TagLib::Ogg::XiphComment *tag = fileRef.xiphComment();
            TagLib::FLAC::Properties *ap = fileRef.audioProperties();

            if(ap && tag && tag->fieldListMap().contains("CUESHEET"))
            {
                qDebug("DecoderFLAC: using cuesheet xiph comment.");
                m_parser = new CueParser(tag->fieldListMap()["CUESHEET"].toString() .toCString(true));
                m_parser->setDuration(fileRef.audioProperties()->lengthInMilliseconds());
                m_parser->setUrl("flac", filePath);
                m_track = m_path.section("#", -1).toInt();
                if(m_track < 1 || m_track > m_parser->count())
                {
                    qWarning("DecoderFLAC: invalid cuesheet xiph comment");
                    return false;
                }
                m_data->input = new QFile(filePath);
                m_data->input->open(QIODevice::ReadOnly);
                if(tag->contains("DISCNUMBER") && !tag->fieldListMap()["DISCNUMBER"].isEmpty())
                {
                    TagLib::StringList fld = tag->fieldListMap()["DISCNUMBER"];
                    for(int i = 1; i <= m_parser->count(); ++i)
                    {
                        m_parser->setMetaData(i, Qmmp::DISCNUMBER, TStringToQString(fld.toString()));
                    }
                }
                addMetaData(m_parser->info(m_track)->metaData()); //send metadata
            }
            else
            {
                qWarning("DecoderFLAC: unable to find cuesheet comment.");
                return false;
            }
        }
        else
        {
            qWarning("DecoderFLAC: cannot initialize. No input.");
            return false;
        }
    }

    if(!m_data->input->isOpen())
    {
        qWarning("DecoderFLAC: unable to open input file");
        return false;
    }

    m_data->bitrate = -1;
    m_data->abort = 0;
    m_data->sample_buffer_fill = 0;
    m_data->last_decode_position = 0;
    m_data->read_bytes = 0;
    m_data->frame_counter = 0;


    if(!m_data->decoder)
    {
        qDebug("DecoderFLAC: creating FLAC__StreamDecoder");
        m_data->decoder = FLAC__stream_decoder_new();
    }
    char buf[500];
    //skip id3v2
    m_data->input->peek(buf, sizeof(buf));
    ulong id3v2_size = findID3v2(buf, sizeof(buf));
    if(id3v2_size)
    {
        qDebug("DecoderFLAC: skipping id3v2 tag (%lu bytes)", id3v2_size);
        m_data->input->seek(id3v2_size);
    }
    m_data->input->peek(buf,sizeof(buf));
    m_data->input->seek(0);
    qDebug("DecoderFLAC: setting callbacks");
    if(!memcmp(buf, "OggS", 4))
    {
        if(!FLAC_API_SUPPORTS_OGG_FLAC)
        {
            qWarning("DecoderFLAC: unsupported format");
            return false;
        }
        if(FLAC__stream_decoder_init_ogg_stream(
                m_data->decoder,
                flac_callback_read,
                flac_callback_seek,
                flac_callback_tell,
                flac_callback_length,
                flac_callback_eof,
                flac_callback_write,
                flac_callback_metadata,
                flac_callback_error,
                m_data) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
        {
            return false;
        }
        qDebug("DecoderFLAC: Ogg FLAC stream found");
        setProperty(Qmmp::FORMAT_NAME, "Ogg FLAC");
    }
    else if(!memcmp(buf, "fLaC", 4))
    {
        if(FLAC__stream_decoder_init_stream(
                m_data->decoder,
                flac_callback_read,
                flac_callback_seek,
                flac_callback_tell,
                flac_callback_length,
                flac_callback_eof,
                flac_callback_write,
                flac_callback_metadata,
                flac_callback_error,
                m_data) != FLAC__STREAM_DECODER_INIT_STATUS_OK)
        {
            return false;
        }
        qDebug("DecoderFLAC: native FLAC stream found");
        setProperty(Qmmp::FORMAT_NAME, "FLAC");
    }
    else
    {
        qWarning("DecoderFLAC: unsupported format");
        return false;
    }

    if(!FLAC__stream_decoder_process_until_end_of_metadata(
                m_data->decoder))
    {
        return false;
    }

    ChannelMap chmap = findChannelMap(m_data->channels);
    if(chmap.isEmpty())
    {
        qWarning("DecoderFLAC: unsupported number of channels: %d", m_data->channels);
        return false;
    }

    switch(m_data->bits_per_sample)
    {
    case 8:
        configure(m_data->sample_rate, chmap, Qmmp::PCM_S8);
        break;
    case 16:
        configure(m_data->sample_rate, chmap, Qmmp::PCM_S16LE);
        break;
    case 24:
    case 32:
        configure(m_data->sample_rate, chmap, Qmmp::PCM_S32LE);
        break;
    default:
        return false;
    }

    if(m_parser)
    {
        m_length = m_parser->duration(m_track);
        m_offset = m_parser->offset(m_track);
        length_in_bytes = audioParameters().sampleRate() *
                          audioParameters().frameSize() * m_length / 1000;
        setReplayGainInfo(m_parser->info(m_track)->replayGainInfo());
        seek(0);
    }

    m_totalBytes = 0;
    m_sz = audioParameters().frameSize();
    qDebug("DecoderFLAC: initialize success");
    return true;
}

qint64 DecoderFLAC::totalTime() const
{
    if(m_parser)
        return m_length;
    return m_data->length;
}

int DecoderFLAC::bitrate() const
{
    return m_data->bitrate;
}

void DecoderFLAC::seek(qint64 time)
{
    m_totalBytes = audioParameters().sampleRate() *
                   audioParameters().channels() *
                   audioParameters().sampleSize() * time / 1000;
    if(m_parser)
        time += m_offset;
    FLAC__uint64 target_sample = FLAC__uint64(time * m_data->total_samples / m_data->length);
    FLAC__stream_decoder_seek_absolute(m_data->decoder, target_sample);

}

qint64 DecoderFLAC::read(unsigned char *data, qint64 maxSize)
{
    if(m_parser)
    {
        if(length_in_bytes - m_totalBytes < m_sz) //end of cue track
            return 0;

        qint64 len = 0;

        if(m_buf) //read remaining data first
        {
            len = qMin(m_buf_size, maxSize);
            memmove(data, m_buf, len);
            if(maxSize >= m_buf_size)
            {
                delete[] m_buf;
                m_buf = nullptr;
                m_buf_size = 0;
            }
            else
                memmove(m_buf, m_buf + len, maxSize - len);
        }
        else
            len = flac_decode (m_data, data, maxSize);

        if(len <= 0) //end of file
            return 0;

        if(len + m_totalBytes <= length_in_bytes)
        {
            m_totalBytes += len;
            return len;
        }

        qint64 len2 = qMax(qint64(0), length_in_bytes - m_totalBytes);
        len2 = (len2 / m_sz) * m_sz; //returned size must contain integer number of samples
        m_totalBytes += len2;
        //save data of the next track
        if(m_buf)
            delete[] m_buf;
        m_buf_size = len - len2;
        m_buf = new char[m_buf_size];
        memmove(m_buf, data + len2, m_buf_size);
        return len2;
    }
    return flac_decode(m_data, data, maxSize);
}

void DecoderFLAC::deinit()
{
    if(m_data->decoder)
        FLAC__stream_decoder_finish(m_data->decoder);

    if(!input() && m_data->input) //delete internal input only
    {
        m_data->input->close();
        delete m_data->input;
        m_data->input = nullptr;
    };
    if(m_parser)
        delete m_parser;
    m_parser = nullptr;
}

const QString DecoderFLAC::nextURL() const
{
    if(m_parser && m_track +1 <= m_parser->count())
        return m_parser->url(m_track + 1);
    else
        return QString();
}

void DecoderFLAC::next()
{
    if(m_parser && m_track +1 <= m_parser->count())
    {
        m_track++;
        m_offset = m_parser->duration(m_track);
        m_length = m_parser->duration(m_track);
        length_in_bytes = audioParameters().sampleRate() *
                          audioParameters().channels() *
                          audioParameters().sampleSize() * m_length / 1000;
        addMetaData(m_parser->info(m_track)->metaData());
        setReplayGainInfo(m_parser->info(m_track)->replayGainInfo());
        m_totalBytes = 0;
    }
}

uint DecoderFLAC::findID3v2(char *data, ulong size) //retuns ID3v2 tag size
{
    if(size < 10)
        return 0;
    if(!memcmp(data, "ID3", 3))
    {
        TagLib::ByteVector byteVector(data, size);
        TagLib::ID3v2::Header header(byteVector);
        return header.completeTagSize();
    }
    return 0;
}

//https://xiph.org/flac/format.html#frame_header
ChannelMap DecoderFLAC::findChannelMap(int channels)
{
    ChannelMap map;
    switch(channels)
    {
    case 1:
        map << Qmmp::CHAN_FRONT_LEFT;
        break;
    case 2:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT;
        break;
    case 3:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER;
        break;
    case 4:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 5:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 6:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_LFE
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT;
        break;
    case 7:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_LFE
            << Qmmp::CHAN_REAR_CENTER
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT;
        break;
    case 8:
        map << Qmmp::CHAN_FRONT_LEFT
            << Qmmp::CHAN_FRONT_RIGHT
            << Qmmp::CHAN_FRONT_CENTER
            << Qmmp::CHAN_LFE
            << Qmmp::CHAN_REAR_LEFT
            << Qmmp::CHAN_REAR_RIGHT
            << Qmmp::CHAN_SIDE_LEFT
            << Qmmp::CHAN_SIDE_RIGHT;
        break;
    default:
        break;
    }
    return map;
}
