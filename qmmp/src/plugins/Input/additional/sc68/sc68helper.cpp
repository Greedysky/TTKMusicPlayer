#include "sc68helper.h"

void in_c68_meta_from_music_info(QVariantMap &data, sc68_music_info_t *ti, int trk)
{
    // add metainfo
    if(!ti->title || !ti->title[0])
    {
        // title is empty, this call will set track title to filename without extension
        data.insert("title", QString());
    }
    else
    {
        data.insert("title", ti->title);
    }

    if(ti->artist && ti->artist[0])
    {
        data.insert("artist", ti->artist);
    }

    if(ti->album && ti->album[0])
    {
        data.insert("album", ti->album);
    }

    if(ti->genre && ti->genre[0])
    {
        data.insert("genre", ti->genre);
    }

    if(ti->year && ti->year[0])
    {
        data.insert("year", ti->year);
    }

    if(ti->format && ti->format[0])
    {
        data.insert("SC68_FORMAT", ti->format);
    }

    if(ti->ripper && ti->ripper[0])
    {
        data.insert("SC68_RIPPER", ti->ripper);
    }

    if(ti->converter && ti->converter[0])
    {
        data.insert("SC68_CONVERTER", ti->converter);
    }

    data.insert("track", trk);
}


SC68Helper::SC68Helper(const QString &path)
    : m_path(path)
{
    m_info = (sc68_info_t*)calloc(sizeof(sc68_info_t), 1);
}

SC68Helper::~SC68Helper()
{
    close();
}

void SC68Helper::close()
{
    if(m_info) 
    {
        if(m_info->sc68)
        {
            sc68_destroy(m_info->sc68);
        }
        free(m_info);
    }
}

bool SC68Helper::initialize()
{
    FILE *file = stdio_open(qPrintable(m_path));
    if(!file)
    {
        return false;
    }

    const int64_t size = stdio_length(file);
    stdio_close(file);

    m_info->sc68 = sc68_create(nullptr);
    if(!m_info->sc68)
    {
        return false;
    }

    // Load an sc68 file.
    int res = sc68_load_uri(m_info->sc68, qPrintable(m_path));
    if(res)
    {
        return false;
    }

    m_info->trk = 0;

    sc68_music_info_t info;
    res = sc68_music_info(m_info->sc68, &info, m_info->trk + 1, 0);
    if(res < 0)
    {
        return false;
    }

    m_info->loop = info.trk.time_ms == 0;
    if(info.trk.time_ms > 0)
    {
        m_info->totalsamples = (uint64_t)info.trk.time_ms * sampleRate() / 1000;
    }
    else
    {
        m_info->totalsamples = 2 * 60 * sampleRate();
    }

    m_totalTime = m_info->totalsamples / sampleRate();
    m_info->bitrate = size * 8.0 / m_totalTime + 0.5;
    m_info->readpos = 0;

    sc68_play(m_info->sc68, m_info->trk + 1, m_info->loop);

    return true;
}

int SC68Helper::totalTime() const
{
    return m_totalTime;
}

void SC68Helper::seek(qint64 time)
{
    const int sample = time * sampleRate();
    if(sample < m_info->currentsample)
    {
        sc68_stop(m_info->sc68);
        sc68_play(m_info->sc68, m_info->trk + 1, m_info->loop);
        m_info->currentsample = 0;
    }

    char buffer[512 * 4];
    while(m_info->currentsample < sample)
    {
        int sz = (int)(sample - m_info->currentsample);
        sz = MIN(sz, sizeof(buffer)>>2);
        int res = sc68_process(m_info->sc68, buffer, &sz);
        if(res & SC68_END)
        {
            break;
        }
        m_info->currentsample += sz;
    }
    m_info->readpos = (float)m_info->currentsample / sampleRate();
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

int SC68Helper::read(unsigned char *buf, int size)
{
    if(m_info->currentsample >= m_info->totalsamples)
    {
        return 0;
    }

    m_info->currentsample += size / (channels() * bitsPerSample() / 8);
    const int initsize = size;

    while(size > 0)
    {
        int n = size>>2;
        int res = sc68_process(m_info->sc68, buf, &n);
        if(res & SC68_END)
        {
            break;
        }
        size -= n<<2;
    }
    return initsize - size;
}

QVariantMap SC68Helper::readMetaTags()
{
    sc68_music_info_t info;
    int err = sc68_music_info(m_info->sc68, &info, 0, 0);
    if(err < 0)
    {
        return m_meta;
    }

    for(int tr = 0; tr < info.tracks; tr++)
    {
        sc68_music_info_t ti;
        memset(&ti, 0, sizeof(ti));
        err = sc68_music_info(m_info->sc68, &ti, tr + 1, 0);
        if(err < 0)
        {
            continue;
        }

        in_c68_meta_from_music_info(m_meta, &ti, tr);
    }

    return m_meta;
}
