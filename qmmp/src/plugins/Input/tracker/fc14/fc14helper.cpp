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
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("FC14Helper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = fc14dec_new();
#ifdef LIBFC_VERSION_OLD
    if(!fc14dec_init(m_input, (void*)buffer.constData(), buffer.length()))
#else
    if(!fc14dec_init(m_input, (void*)buffer.constData(), buffer.length(), 0))
#endif
    {
        qWarning("FC14Helper: fc14dec_init error");
        return false;
    }

    // Initialize decoder's audio sample mixer.  frequency : output sample frequency
    // precision : bits per sample  channels : 1=mono, 2=stereo
    // zero : value of silent output sample (e.g. 0x80 for unsigned 8-bit, 0x0000 for signed 16-bit)
#ifdef LIBFC_VERSION_OLD
    fc14dec_mixer_init(m_input, sampleRate(), depth(), channels(), 0x0000);
#else
    // panning : 100 to 0, default = 75 100=full stereo, 50=middle, 0=mirrored full stereo
    fc14dec_mixer_init(m_input, sampleRate(), depth(), channels(), 0x0000, 75);
#endif
    return true;
}

qint64 FC14Helper::read(unsigned char *data, qint64 maxSize)
{
    fc14dec_buffer_fill(m_input, data, maxSize);
    return fc14dec_song_end(m_input) ? 0 : maxSize;
}
