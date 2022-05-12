#include "fc14helper.h"

FC14Helper::FC14Helper(const QString &path)
    : m_path(path)
{

}

FC14Helper::~FC14Helper()
{
    deinit();
}

void FC14Helper::deinit()
{
    if(m_input)
    {
        fc14dec_delete(m_input);
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
    const QByteArray &module = file.readAll();
    file.close();

    m_input = fc14dec_new();
    if(!fc14dec_detect(m_input, (void*)module.constData(), size))
    {
        qWarning("FC14Helper: fc14dec_detect error");
        return false;
    }

    if(!fc14dec_init(m_input, (void*)module.constData(), size))
    {
        qWarning("FC14Helper: fc14dec_init error");
        return false;
    }

    // Initialize decoder's audio sample mixer.  frequency : output sample frequency
    // precision : bits per sample  channels : 1=mono, 2=stereo
    // zero : value of silent output sample (e.g. 0x80 for unsigned 8-bit, 0x0000 for signed 16-bit)
    fc14dec_mixer_init(m_input, sampleRate(), depth(), channels(), 0x0000);
    return true;
}

qint64 FC14Helper::read(unsigned char *data, qint64 maxSize)
{
    fc14dec_buffer_fill(m_input, data, maxSize);
    return fc14dec_song_end(m_input) ? 0 : maxSize;
}
