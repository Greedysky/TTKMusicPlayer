#include "sc68helper.h"

#include <QFile>

void in_c68_meta_from_music_info(TrackInfo *info, sc68_music_info_t *ti)
{
    // add metainfo
    if(!ti->title || !ti->title[0])
    {
        // title is empty, this call will set track title to filename without extension
        info->setValue(Qmmp::TITLE, QString());
    }
    else
    {
        info->setValue(Qmmp::TITLE, ti->title);
    }

    if(ti->artist && ti->artist[0])
    {
        info->setValue(Qmmp::ARTIST, ti->artist);
    }

    if(ti->album && ti->album[0])
    {
        info->setValue(Qmmp::ALBUM, ti->album);
    }

    if(ti->genre && ti->genre[0])
    {
        info->setValue(Qmmp::GENRE, ti->genre);
    }

    if(ti->year && ti->year[0])
    {
        info->setValue(Qmmp::YEAR, ti->year);
    }

    if(ti->format && ti->format[0])
    {
        info->setValue(Qmmp::COMMENT, ti->format);
    }
}


SC68Helper::SC68Helper(const QString &path)
    : m_path(path)
{
    m_info = (decode_info*)calloc(sizeof(decode_info), 1);
}

SC68Helper::~SC68Helper()
{
    deinit();
}

void SC68Helper::deinit()
{
    if(m_info->input)
    {
        sc68_destroy(m_info->input);
    }
    free(m_info);
}

bool SC68Helper::initialize()
{
    const int track = m_path.section("#", -1).toInt();
    QString path = cleanPath();

    QFile file(path);
    if(!file.open(QFile::ReadOnly))
    {
        qWarning("SC68Helper: open file failed");
        return false;
    }

    const qint64 size = file.size();
    sc68_init(nullptr);

    m_info->input = sc68_create(nullptr);
    if(!m_info->input)
    {
        qWarning("SC68Helper: sc68_create error");
        return false;
    }

    int res = sc68_load_uri(m_info->input, Qmmp::generatePrintable("file://" + path));
    if(res)
    {
        qWarning("SC68Helper: sc68_load_uri error");
        return false;
    }

    sc68_music_info_t info;
    res = sc68_music_info(m_info->input, &info, 0, 0);
    if(res < 0)
    {
        qWarning("SC68Helper: sc68_music_info error");
        return false;
    }

    if(track > info.tracks || track < 0)
    {
        qWarning("SC68Helper: track number is out of range");
        return false;
    }

    m_info->loop = (info.trk.time_ms == 0);
    m_info->track = track;

    if(info.trk.time_ms > 0)
    {
        m_info->total_samples = (uint64_t)info.trk.time_ms * sampleRate() / 1000;
    }
    else
    {
        m_info->total_samples = 2 * 60 * sampleRate();
    }

    m_info->length = m_info->total_samples / sampleRate() * 1000;
    m_info->bitrate = size * 8.0 / totalTime() + 1.0f;

    sc68_play(m_info->input, m_info->track, m_info->loop);

    return true;
}

qint64 SC68Helper::totalTime() const
{
    return m_info->length;
}

void SC68Helper::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample < m_info->current_sample)
    {
        sc68_stop(m_info->input);
        sc68_play(m_info->input, m_info->track, m_info->loop);
        m_info->current_sample = 0;
    }

    char buffer[512 * 4];
    while(m_info->current_sample < sample)
    {
        int sz = (int)(sample - m_info->current_sample);
        sz = std::min<int>(sz, sizeof(buffer) >> 2);

        if(sc68_process(m_info->input, buffer, &sz) & SC68_END)
        {
            break;
        }
        m_info->current_sample += sz;
    }
}

int SC68Helper::bitrate() const
{
    return m_info->bitrate;
}

int SC68Helper::sampleRate() const
{
    return 44100;
}

int SC68Helper::channels() const
{
    return 2;
}

int SC68Helper::bitsPerSample() const
{
    return 16;
}

qint64 SC68Helper::read(unsigned char *data, qint64 maxSize)
{
    if(m_info->current_sample >= m_info->total_samples)
    {
        return 0;
    }

    m_info->current_sample += maxSize / (channels() * bitsPerSample() / 8);
    const int initSize = maxSize;

    while(maxSize > 0)
    {
        int n = maxSize >> 2;
        if(sc68_process(m_info->input, data, &n) & SC68_END)
        {
            break;
        }
        maxSize -= n << 2;
    }

    return initSize - maxSize;
}

QList<TrackInfo*> SC68Helper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> list;
    if(!m_info->input)
    {
        return list;
    }

    sc68_music_info_t info;
    if(sc68_music_info(m_info->input, &info, 0, 0) < 0)
    {
        return list;
    }

    for(int i = 1; i <= info.tracks; i++)
    {
        sc68_music_info_t ti;
        memset(&ti, 0, sizeof(ti));
        if(sc68_music_info(m_info->input, &ti, i, 0) < 0)
        {
            continue;
        }

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            in_c68_meta_from_music_info(info, &ti);
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, bitsPerSample());
            info->setValue(Qmmp::FORMAT_NAME, "SC68");
        }

        info->setPath("sc68://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(ti.trk.time_ms > 0 ? ti.trk.time_ms : (2 * 60));
        list << info;
    }
    return list;
}

QString SC68Helper::cleanPath() const
{
    QString path = m_path;
    if(m_path.contains("://"))
    {
        path.remove("sc68://");
        path.remove(RegularWrapper("#\\d+$"));
    }
    return path;
}
