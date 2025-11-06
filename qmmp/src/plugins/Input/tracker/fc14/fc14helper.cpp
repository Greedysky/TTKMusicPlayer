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
    QFile file(cleanPath());
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning("FC14Helper: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = fc14dec_new();
    if(!fc14dec_detect(m_input, (void*)buffer.constData(), buffer.length()))
    {
        qWarning("FC14Helper: fc14dec_detect error");
        return false;
    }

#ifdef LIBFC_VERSION1
    if(!fc14dec_init(m_input, (void*)buffer.constData(), buffer.length()))
#else
    const int track = m_path.section("#", -1).toInt() - 1;
    if(!fc14dec_init(m_input, (void*)buffer.constData(), buffer.length(), track < 0 ? 0 : track))
#endif
    {
        qWarning("FC14Helper: fc14dec_init error");
        return false;
    }

    // Initialize decoder's audio sample mixer.  frequency : output sample frequency
    // precision : bits per sample  channels : 1=mono, 2=stereo
    // zero : value of silent output sample (e.g. 0x80 for unsigned 8-bit, 0x0000 for signed 16-bit)
#ifdef LIBFC_VERSION1
    fc14dec_mixer_init(m_input, sampleRate(), depth(), channels(), 0x0000);
#else
    // panning : 100 to 0, default = 75 100=full stereo, 50=middle, 0=mirrored full stereo
    fc14dec_mixer_init(m_input, sampleRate(), depth(), channels(), 0x0000, 100);
#endif
    return true;
}

qint64 FC14Helper::read(unsigned char *data, qint64 maxSize)
{
    fc14dec_buffer_fill(m_input, data, maxSize);
    return fc14dec_song_end(m_input) ? 0 : maxSize;
}

QList<TrackInfo*> FC14Helper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

#ifdef LIBFC_VERSION1
    const int songs = 1;
#else
    const int songs = fc14dec_songs(m_input);
#endif
    for(int i = 1; i <= songs; ++i)
    {
        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TRACK, i);
            info->setValue(Qmmp::COMMENT, comment());
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, "Future Composer");
        }

        info->setPath("fc14://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }
    return playlist;
}

QString FC14Helper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}
