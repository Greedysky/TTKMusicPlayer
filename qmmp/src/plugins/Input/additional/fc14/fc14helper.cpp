#include "fc14helper.h"

FC14Helper::FC14Helper(const QString &path)
    : m_path(path)
{
    m_info = (fc14_info_t*)calloc(sizeof(fc14_info_t), 1);
}

FC14Helper::~FC14Helper()
{
    close();
}

void FC14Helper::close()
{
    if(m_info)
    {
        if(m_info->fc)
        {
            fc14dec_delete(m_info->fc);
        }
        free(m_info);
    }
}

bool FC14Helper::initialize()
{
    FILE *file = stdio_open(qPrintable(m_path));
    if(!file)
    {
        qWarning("FC14Helper: open file failed");
        return false;
    }

    int size = stdio_length(file);
    if(size <= 0 || size > 256 * 1024)
    {
        qWarning("FC14Helper: file size invalid");
        stdio_close(file);
        return false;
    }

    unsigned char *module = (unsigned char *)malloc(size);
    if(!module)
    {
        qWarning("FC14Helper: file data read error");
        stdio_close(file);
        return false;
    }

    stdio_read(module, size, 1, file);
    stdio_close(file);

    m_info->fc = fc14dec_new();
    if(!fc14dec_detect(m_info->fc, module, size))
    {
        qWarning("FC14Helper: fc14dec_detect error");
        free(module);
        return false;
    }

    if(!fc14dec_init(m_info->fc, module, size))
    {
        qWarning("FC14Helper: fc14dec_init error");
        free(module);
        return false;
    }
    free(module);

    // Initialize decoder's audio sample mixer.  frequency : output sample frequency
    // precision : bits per sample  channels : 1=mono, 2=stereo
    // zero : value of silent output sample (e.g. 0x80 for unsigned 8-bit, 0x0000 for signed 16-bit)
    fc14dec_mixer_init(m_info->fc, 44100, 16, 2, 0x0000);

    m_info->length = fc14dec_duration(m_info->fc);
    m_info->channels = 2;
    m_info->sample_rate = 44100;
    m_info->bits_per_sample = 16;
    m_info->bitrate = size * 8.0 / m_info->length + 1.0f;

    return true;
}

int FC14Helper::totalTime() const
{
    return m_info->length;
}

void FC14Helper::seek(qint64 time)
{
    fc14dec_seek(m_info->fc, time);
}

int FC14Helper::bitrate() const
{
    return m_info->bitrate;
}

int FC14Helper::sampleRate() const
{
    return m_info->sample_rate;
}

int FC14Helper::channels() const
{
    return m_info->channels;
}

int FC14Helper::bitsPerSample() const
{
    return m_info->bits_per_sample;
}

int FC14Helper::read(unsigned char *buf, int size)
{
    fc14dec_buffer_fill(m_info->fc, buf, size);
    if(fc14dec_song_end(m_info->fc))
    {
        return 0;
    }
    return size;
}

QVariantMap FC14Helper::readMetaTags()
{
    QVariantMap data;
    data.insert("comment", fc14dec_format_name(m_info->fc));
    return data;
}
