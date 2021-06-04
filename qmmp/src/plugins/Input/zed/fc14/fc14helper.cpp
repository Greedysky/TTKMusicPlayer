#include "fc14helper.h"

FC14Helper::FC14Helper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

FC14Helper::~FC14Helper()
{
    deinit();
}

void FC14Helper::deinit()
{
    if(m_info)
    {
        if(m_info->input)
        {
            fc14dec_delete(m_info->input);
        }
        free(m_info);
    }
}

bool FC14Helper::initialize()
{
    QFile file(m_path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("FC14Helper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    const QByteArray module = file.readAll();

    m_info->input = fc14dec_new();
    if(!fc14dec_detect(m_info->input, (void*)module.constData(), size))
    {
        qWarning("FC14Helper: fc14dec_detect error");
        return false;
    }

    if(!fc14dec_init(m_info->input, (void*)module.constData(), size))
    {
        qWarning("FC14Helper: fc14dec_init error");
        return false;
    }

    // Initialize decoder's audio sample mixer.  frequency : output sample frequency
    // precision : bits per sample  channels : 1=mono, 2=stereo
    // zero : value of silent output sample (e.g. 0x80 for unsigned 8-bit, 0x0000 for signed 16-bit)
    fc14dec_mixer_init(m_info->input, sampleRate(), bitsPerSample(), channels(), 0x0000);
    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;
    return true;
}

int FC14Helper::totalTime() const
{
    return fc14dec_duration(m_info->input);
}

void FC14Helper::seek(qint64 time)
{
    fc14dec_seek(m_info->input, time);
}

int FC14Helper::bitrate() const
{
    return m_info->bitrate;
}

int FC14Helper::sampleRate() const
{
    return 44100;
}

int FC14Helper::channels() const
{
    return 2;
}

int FC14Helper::bitsPerSample() const
{
    return 16;
}

int FC14Helper::read(unsigned char *buf, int size)
{
    fc14dec_buffer_fill(m_info->input, buf, size);
    if(fc14dec_song_end(m_info->input))
    {
        return 0;
    }
    return size;
}

QMap<Qmmp::MetaData, QString> FC14Helper::readMetaData() const
{
    QMap<Qmmp::MetaData, QString> metaData;
    metaData.insert(Qmmp::COMMENT, fc14dec_format_name(m_info->input));
    return metaData;
}
