extern "C" {
#define __STDC_CONSTANT_MACROS
#define __STDC_LIMIT_MACROS
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/mathematics.h>
}

#include "spek-audio.h"

#ifdef Q_CC_GNU
#  pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

class AudioFileImpl : public AudioFile
{
public:
    AudioFileImpl(
        AudioError error, AVFormatContext *format_context, AVCodecContext *codec_context,
        int audio_stream, const std::string& codec_name, int bit_rate, int sample_rate,
        int bits_per_sample, int streams, int channels, double duration
    );
    ~AudioFileImpl();

    virtual void start(int channel, int samples) override final;
    virtual int read() override final;

    AudioError get_error() const override { return m_error; }
    std::string get_codec_name() const override { return m_codec_name; }
    int get_bit_rate() const override { return m_bit_rate; }
    int get_sample_rate() const override { return m_sample_rate; }
    int get_bits_per_sample() const override { return m_bits_per_sample; }
    int get_streams() const override { return m_streams; }
    int get_channels() const override { return m_channels; }
    double get_duration() const override { return m_duration; }
    const float *get_buffer() const override { return m_buffer; }
    int64_t get_frames_per_interval() const override { return m_frames_per_interval; }
    int64_t get_error_per_interval() const override { return m_error_per_interval; }
    int64_t get_error_base() const override { return m_error_base; }

private:
    AudioError m_error;
    AVFormatContext *m_format_context;
    AVCodecContext *m_codec_context;
    int m_audio_stream;
    std::string m_codec_name;
    int m_bit_rate;
    int m_sample_rate;
    int m_bits_per_sample;
    int m_streams;
    int m_channels;
    double m_duration;

    int m_channel;

    AVPacket *m_packet;
    int m_offset = 0;
    AVFrame *m_frame;
    int m_buffer_len = 0;
    float *m_buffer = nullptr;
    // TODO: these guys don't belong here, move them somewhere else when revamping the pipeline
    int64_t m_frames_per_interval = 0;
    int64_t m_error_per_interval = 0;
    int64_t m_error_base = 0;

};


Audio::Audio()
{
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 10, 100) //ffmpeg-3.5
    av_register_all();
#endif
}

Audio::~Audio()
{
#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(58, 10, 100) //ffmpeg-3.5
    // This prevents a memory leak.
    av_lockmgr_register(nullptr);
#endif
}

std::unique_ptr<AudioFile> Audio::open(const std::string& file_name, int stream)
{
    AudioError error = AudioError::OK;

    AVFormatContext *format_context = nullptr;
    if(avformat_open_input(&format_context, file_name.c_str(), nullptr, nullptr) < 0) {
        error = AudioError::CANNOT_OPEN_FILE;
    }

    if(!error && avformat_find_stream_info(format_context, nullptr) < 0) {
        // 24-bit APE returns an error but parses the stream info just fine.
        // TODO: old comment, verify
        if(format_context->nb_streams <= 0) {
            error = AudioError::NO_STREAMS;
        }
    }

    int audio_stream = -1;
    int streams = 0;
    if(!error) {
        for(unsigned int i = 0; i < format_context->nb_streams; ++i) {
            if(format_context->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
                if(stream == streams) {
                    audio_stream = i;
                }
                streams++;
            }
        }
        if(audio_stream == -1) {
            error = AudioError::NO_AUDIO;
        }
    }

    AVStream *avstream = nullptr;
    AVCodecParameters *codecpar = nullptr;
    const AVCodec *codec = nullptr;
    if(!error) {
        avstream = format_context->streams[audio_stream];
        codecpar = avstream->codecpar;
        codec = avcodec_find_decoder(codecpar->codec_id);
        if(!codec) {
            error = AudioError::NO_DECODER;
        }
    }

    std::string codec_name;
    int bit_rate = 0;
    int sample_rate = 0;
    int bits_per_sample = 0;
    int channels = 0;
    double duration = 0;
    if(!error) {
        // We can already fill in the stream info even if the codec won't be able to open it.
        if(codec->long_name) {
            codec_name = codec->long_name;
        } else if(codec->name) {
            codec_name = codec->name;
        }
        bit_rate = codecpar->bit_rate;
        sample_rate = codecpar->sample_rate;
        bits_per_sample = codecpar->bits_per_raw_sample;
        if(!bits_per_sample) {
            // APE uses bpcs, FLAC uses bprs.
            bits_per_sample = codecpar->bits_per_coded_sample;
        }
        if(codecpar->codec_id == AV_CODEC_ID_AAC ||
            codecpar->codec_id == AV_CODEC_ID_MUSEPACK8 ||
            codecpar->codec_id == AV_CODEC_ID_WMAV1 ||
            codecpar->codec_id == AV_CODEC_ID_WMAV2) {
            // These decoders set both bps and bitrate.
            bits_per_sample = 0;
        }
        if(bits_per_sample) {
            bit_rate = 0;
        }
#if LIBAVCODEC_VERSION_INT >= AV_VERSION_INT(59, 37, 100) //ffmpeg-5.1
        channels = codecpar->ch_layout.nb_channels;
#else
        channels = codecpar->channels;
#endif

        if(avstream->duration != AV_NOPTS_VALUE) {
            duration = avstream->duration * av_q2d(avstream->time_base);
        } else if(format_context->duration != AV_NOPTS_VALUE) {
            duration = format_context->duration / (double) AV_TIME_BASE;
        } else {
            error = AudioError::NO_DURATION;
        }

        if(!error && channels <= 0) {
            error = AudioError::NO_CHANNELS;
        }
    }

    AVCodecContext *codec_context = nullptr;
    if(!error) {
        error = AudioError::CANNOT_OPEN_DECODER;
        // Allocate a codec context for the decoder.
        codec_context = avcodec_alloc_context3(codec);
        if(codec_context) {
            // Copy codec parameters from input stream to output codec context.
            if(avcodec_parameters_to_context(codec_context, codecpar) == 0) {
                // Finally, init the decoder.
                if(avcodec_open2(codec_context, codec, nullptr) == 0) {
                    error = AudioError::OK;
                }
            }
        }
    }

    if(!error) {
        AVSampleFormat fmt = (AVSampleFormat)codecpar->format;
        if(fmt != AV_SAMPLE_FMT_S16 && fmt != AV_SAMPLE_FMT_S16P &&
            fmt != AV_SAMPLE_FMT_S32 && fmt != AV_SAMPLE_FMT_S32P &&
            fmt != AV_SAMPLE_FMT_FLT && fmt != AV_SAMPLE_FMT_FLTP &&
            fmt != AV_SAMPLE_FMT_DBL && fmt != AV_SAMPLE_FMT_DBLP ) {
            error = AudioError::BAD_SAMPLE_FORMAT;
        }
    }

    return std::unique_ptr<AudioFile>(new AudioFileImpl(
        error, format_context, codec_context,
        audio_stream, codec_name, bit_rate, sample_rate,
        bits_per_sample, streams, channels, duration
    ));
}

AudioFileImpl::AudioFileImpl(
    AudioError error, AVFormatContext *format_context, AVCodecContext *codec_context,
    int audio_stream, const std::string& codec_name, int bit_rate, int sample_rate,
    int bits_per_sample, int streams, int channels, double duration)
    : m_error(error),
      m_format_context(format_context),
      m_codec_context(codec_context),
      m_audio_stream(audio_stream),
      m_codec_name(codec_name),
      m_bit_rate(bit_rate),
      m_sample_rate(sample_rate),
      m_bits_per_sample(bits_per_sample),
      m_streams(streams),
      m_channels(channels),
      m_duration(duration)
{
    m_packet = av_packet_alloc();
    m_packet->data = nullptr;
    m_packet->size = 0;
    m_frame = av_frame_alloc();
}

AudioFileImpl::~AudioFileImpl()
{
    if(m_buffer) {
        av_freep(&m_buffer);
    }
    if(m_frame) {
        av_frame_free(&m_frame);
    }
    if(m_packet->data) {
        m_packet->data -= m_offset;
        m_packet->size += m_offset;
        m_offset = 0;
        av_packet_unref(m_packet);
    }
    if(m_codec_context) {
        avcodec_free_context(&m_codec_context);
    }
    if(m_format_context) {
        avformat_close_input(&m_format_context);
    }
}

void AudioFileImpl::start(int channel, int samples)
{
    m_channel = channel;
    if(channel < 0 || channel >= m_channels) {
        m_error = AudioError::NO_CHANNELS;
    }

    AVStream *stream = m_format_context->streams[m_audio_stream];
    int64_t rate = m_sample_rate * (int64_t)stream->time_base.num;
    int64_t duration = (int64_t)(m_duration * stream->time_base.den / stream->time_base.num);
    m_error_base = samples * (int64_t)stream->time_base.den;
    m_frames_per_interval = av_rescale_rnd(duration, rate, m_error_base, AV_ROUND_DOWN);
    m_error_per_interval = (duration * rate) % m_error_base;
}

int AudioFileImpl::read()
{
    if(!!m_error) {
        return -1;
    }

    for(;;) {
        while(m_packet->size > 0) {
            av_frame_unref(m_frame);
            int ret = avcodec_send_packet(m_codec_context, m_packet);
            if(ret < 0) {
                // Error sending a packet for decoding, skip the frame.
                break;
            }
            ret = avcodec_receive_frame(m_codec_context, m_frame);
            if(ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) {
                // Error during decoding, skip the frame.
                break;
            }
            const int len = m_packet->size;
            m_packet->data += len;
            m_packet->size -= len;
            m_offset += len;
            const int samples = m_frame->nb_samples;
            // Occasionally the frame has no samples, move on to the next one.
            if(samples == 0) {
                continue;
            }
            // We have data, return it and come back for more later.
            if(samples > m_buffer_len) {
                m_buffer = static_cast<float*>(
                    av_realloc(m_buffer, samples * sizeof(float))
                );
                m_buffer_len = samples;
            }

            AVSampleFormat format = static_cast<AVSampleFormat>(m_frame->format);
            int is_planar = av_sample_fmt_is_planar(format);
            for(int sample = 0; sample < samples; ++sample) {
                uint8_t *data;
                int offset;
                if(is_planar) {
                    data = m_frame->data[m_channel];
                    offset = sample;
                } else {
                    data = m_frame->data[0];
                    offset = sample * m_channels;
                }
                float value;
                switch(format) {
                case AV_SAMPLE_FMT_S16:
                case AV_SAMPLE_FMT_S16P:
                    value = reinterpret_cast<int16_t*>(data)[offset]
                        / static_cast<float>(INT16_MAX);
                    break;
                case AV_SAMPLE_FMT_S32:
                case AV_SAMPLE_FMT_S32P:
                    value = reinterpret_cast<int32_t*>(data)[offset]
                        / static_cast<float>(INT32_MAX);
                    break;
                case AV_SAMPLE_FMT_FLT:
                case AV_SAMPLE_FMT_FLTP:
                    value = reinterpret_cast<float*>(data)[offset];
                    break;
                case AV_SAMPLE_FMT_DBL:
                case AV_SAMPLE_FMT_DBLP:
                    value = reinterpret_cast<double*>(data)[offset];
                    break;
                default:
                    value = 0.0f;
                    break;
                }
                m_buffer[sample] = value;
            }
            return samples;
        }
        if(m_packet->data) {
            m_packet->data -= m_offset;
            m_packet->size += m_offset;
            m_offset = 0;
            av_packet_unref(m_packet);
        }

        int res = 0;
        while((res = av_read_frame(m_format_context, m_packet)) >= 0) {
            if(m_packet->stream_index == m_audio_stream) {
                break;
            }
            av_packet_unref(m_packet);
        }
        if(res < 0) {
            // End of file or error.
            return 0;
        }
    }
}
