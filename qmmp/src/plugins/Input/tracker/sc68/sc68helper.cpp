#include "sc68helper.h"

SC68Helper::SC68Helper(const QString &path)
    : m_path(path)
{

}

SC68Helper::~SC68Helper()
{
    deinit();
}

void SC68Helper::deinit()
{
    if(m_input)
    {
        sc68_destroy(m_input);
    }
}

bool SC68Helper::initialize()
{
    const QString &path = cleanPath();

    sc68_init(nullptr);
    m_input = sc68_create(nullptr);
    if(!m_input)
    {
        qWarning("SC68Helper: sc68_create error");
        return false;
    }

    if(sc68_load_uri(m_input, QmmpPrintable("file://" + path)))
    {
        qWarning("SC68Helper: unable to open file");
        return false;
    }

    sc68_music_info_t info;
    if(sc68_music_info(m_input, &info, 0, 0) < 0)
    {
        qWarning("SC68Helper: sc68_music_info error");
        return false;
    }

    const int track = m_path.section("#", -1).toInt();
    if(track > info.tracks || track < 0)
    {
        qWarning("SC68Helper: track number is out of range");
        return false;
    }

    m_loop = (info.trk.time_ms == 0);
    m_track = track;

    if(info.trk.time_ms > 0)
    {
        m_totalSamples = (uint64_t)info.trk.time_ms * sampleRate() / 1000;
    }
    else
    {
        m_totalSamples = 2 * 60 * sampleRate();
    }

    m_length = m_totalSamples / sampleRate() * 1000;

    sc68_play(m_input, m_track, m_loop);
    return true;
}

void SC68Helper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample < m_currentSample)
    {
        sc68_stop(m_input);
        sc68_play(m_input, m_track, m_loop);
        m_currentSample = 0;
    }

    char buffer[512 * 4];
    while(m_currentSample < sample)
    {
        int n = (int)(sample - m_currentSample);
        n = std::min<int>(n, sizeof(buffer) >> 2);

        if(sc68_process(m_input, buffer, &n) & SC68_END)
        {
            break;
        }
        m_currentSample += n;
    }
}

qint64 SC68Helper::read(unsigned char *data, qint64 maxSize)
{
    if(m_currentSample >= m_totalSamples)
    {
        return 0;
    }

    m_currentSample += maxSize / (channels() * depth() / 8);
    const int initSize = maxSize;

    while(maxSize > 0)
    {
        int n = maxSize >> 2;
        if(sc68_process(m_input, data, &n) & SC68_END)
        {
            break;
        }
        maxSize -= n << 2;
    }

    return initSize - maxSize;
}

QList<TrackInfo*> SC68Helper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    sc68_music_info_t info;
    if(sc68_music_info(m_input, &info, 0, 0) < 0)
    {
        return playlist;
    }

    for(int i = 1; i <= info.tracks; ++i)
    {
        sc68_music_info_t tag;
        memset(&tag, 0, sizeof(tag));
        if(sc68_music_info(m_input, &tag, i, 0) < 0)
        {
            continue;
        }

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            // add metainfo
            if(tag.title && tag.title[0])
            {
                info->setValue(Qmmp::TITLE, tag.title);
            }
            else if(tag.artist && tag.artist[0])
            {
                info->setValue(Qmmp::ARTIST, tag.artist);
            }
            else if(tag.album && tag.album[0])
            {
                info->setValue(Qmmp::ALBUM, tag.album);
            }
            else if(tag.genre && tag.genre[0])
            {
                info->setValue(Qmmp::GENRE, tag.genre);
            }
            else if(tag.year && tag.year[0])
            {
                info->setValue(Qmmp::YEAR, tag.year);
            }
            else if(tag.format && tag.format[0])
            {
                info->setValue(Qmmp::COMMENT, tag.format);
            }

            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, "SC68 Amiga");
        }

        info->setPath("sc68://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(tag.trk.time_ms > 0 ? tag.trk.time_ms : (2 * 60));
        playlist << info;
    }
    return playlist;
}

QString SC68Helper::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}
