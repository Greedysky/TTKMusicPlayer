#include "xgmhelper.h"

#include <QSettings>
#include <libxgm/kss/kssplay.h>
#include <libxgm/nezplug/nezplug.h>
#include <libxgm/jaytrax/jxs.h>
#include <libxgm/pac/pacP.h>

AbstractReader::AbstractReader(const QString &path)
    : m_path(path)
{

}

QString AbstractReader::cleanPath() const
{
    return m_path.contains("://") ? TrackInfo::pathFromUrl(m_path) : m_path;
}


class KssReader : public AbstractReader
{
public:
    KssReader(const QString &path);
    ~KssReader();

    virtual bool load() override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;
    virtual QList<TrackInfo*> createPlayList(TrackInfo::Parts parts) override final;

private:
    int m_loop = 0;
    int m_track = 0;
    KSS *m_kss = nullptr;
    KSSPLAY *m_input = nullptr;

};

KssReader::KssReader(const QString &path)
    : AbstractReader(path)
{

}

KssReader::~KssReader()
{
    if(m_input)
    {
        KSSPLAY_delete(m_input);
    }

    if(m_kss)
    {
        KSS_delete(m_kss);
    }
}

bool KssReader::load()
{
    const QString &path = cleanPath();

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
//        qWarning("KssReader: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_kss = KSS_bin2kss((uint8_t*)buffer.constData(), buffer.length(), QmmpPrintable(path));
    if(!m_kss)
    {
//        qWarning("KssReader: KSS_bin2kss failed");
        return false;
    }

    m_input = KSSPLAY_new(sampleRate(), channels(), depth());
    if(!m_input)
    {
//        qWarning("KssReader: KSSPLAY_new failed");
        return false;
    }

    KSSPLAY_set_data(m_input, m_kss);

    m_track = m_path.section("#", -1).toInt() - 1;
    if(m_track < 0)
    {
        m_track = 0;
    }

    KSSPLAY_reset(m_input, m_track, 0);

    const uint32_t quality = 1;
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_PSG, quality);
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_SCC, quality);
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_OPL, quality);
    KSSPLAY_set_device_quality(m_input, KSS_DEVICE_OPLL, quality);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("XGM");
    const int master = settings.value("kss_master_volume", 32).toInt();
    const int pan = settings.value("kss_device_pan", 32).toInt();
    const int silence = settings.value("kss_silence", 5).toInt();
    settings.endGroup();

    KSSPLAY_set_master_volume(m_input, master);

    KSSPLAY_set_device_pan(m_input, KSS_DEVICE_PSG, -pan);
    KSSPLAY_set_device_pan(m_input, KSS_DEVICE_SCC, pan);

    m_input->opll_stereo = 1;
    m_input->silent_limit = silence * 1000;

    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 0, 1);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 1, 2);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 2, 1);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 3, 2);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 4, 1);
    KSSPLAY_set_channel_pan(m_input, KSS_DEVICE_OPLL, 5, 2);

    m_length = 150 * 1000;
    m_totalSamples = totalTime() * sampleRate() / 1000;
    return true;
}

void KssReader::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample < m_currentSample)
    {
        KSSPLAY_reset(m_input, m_track, 0);
        m_currentSample = 0;
    }

    if(m_currentSample != sample)
    {
        KSSPLAY_calc_silent(m_input, sample - m_currentSample);
        m_currentSample = sample;
    }
}

qint64 KssReader::read(unsigned char *data, qint64 maxSize)
{
    if(m_currentSample >= m_totalSamples)
    {
        return 0;
    }

    const int loop = KSSPLAY_get_loop_count(m_input);
    if(loop != m_loop || KSSPLAY_get_stop_flag(m_input))
    {
        m_loop = loop;
        return 0;
    }

    const int size = maxSize / (channels() * depth() / 8);
    m_currentSample += size;
    KSSPLAY_calc(m_input, (int16_t*)data, size);
    return maxSize;
}

QList<TrackInfo*> KssReader::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    int songs = m_kss->trk_max - m_kss->trk_min + 1;
    if(songs < 1)
    {
        songs = 1;
    }

    for(int i = 1; i <= songs; ++i)
    {
        KSSPLAY_reset(m_input, i - 1, 0);

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, KSS_get_title(m_kss));
            info->setValue(Qmmp::COMMENT, reinterpret_cast<char*>(m_kss->extra));
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, reinterpret_cast<char*>(m_kss->idstr));
        }

        info->setPath("xgm://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }
    return playlist;
}


class NEZplugReader : public AbstractReader
{
public:
    NEZplugReader(const QString &path);
    ~NEZplugReader();

    virtual bool load() override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;
    virtual QList<TrackInfo*> createPlayList(TrackInfo::Parts parts) override final;

private:
    NEZ_PLAY *m_input = nullptr;

};

NEZplugReader::NEZplugReader(const QString &path)
    : AbstractReader(path)
{

}

NEZplugReader::~NEZplugReader()
{
    if(m_input)
    {
        NEZDelete(m_input);
    }
}

bool NEZplugReader::load()
{
    QFile file(cleanPath());
    if(!file.open(QIODevice::ReadOnly))
    {
//        qWarning("NEZplugReader: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    m_input = NEZNew();
    if(!m_input)
    {
//        qWarning("NEZplugReader: NEZNew failed");
        return false;
    }

    if(NEZLoad(m_input, (uint8_t*)buffer.constData(), buffer.length()))
    {
//        qWarning("NEZplugReader: NEZLoad failed");
        return false;
    }

    NEZSetFrequency(m_input, sampleRate());
    NEZSetChannel(m_input, channels());

    const int track = m_path.section("#", -1).toInt();
    NEZSetSongNo(m_input, track < 1 ? 1 : track);
    NEZReset(m_input);

    QSettings settings(Qmmp::configFile(), QSettings::IniFormat);
    settings.beginGroup("XGM");
    const int filter = settings.value("nez_filter", 0).toInt();
    const int gain = settings.value("nez_gain", 3).toInt();
    settings.endGroup();

    NEZSetFilter(m_input, filter);
    NEZGain(m_input, int32_t(255.0f * (gain - 1) / 7.0f));

    m_length = NEZGetTrackLength(m_input, track < 1 ? 1 : track);
    m_totalSamples = totalTime() * sampleRate() / 1000;
    return true;
}

void NEZplugReader::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample < m_currentSample)
    {
        NEZReset(m_input);
        m_currentSample = 0;
    }

    if(m_currentSample != sample)
    {
        NEZRender(m_input, nullptr, sample - m_currentSample);
        m_currentSample = sample;
    }
}

qint64 NEZplugReader::read(unsigned char *data, qint64 maxSize)
{
    if(m_currentSample >= m_totalSamples)
    {
        return 0;
    }

    const int size = maxSize / (channels() * depth() / 8);
    m_currentSample += size;
    NEZRender(m_input, data, size);
    return maxSize;
}

QList<TrackInfo*> NEZplugReader::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    const int songs = NEZGetSongMaxAbsolute(m_input) - NEZGetSongStart(m_input) + 1;
    for(int i = 1; i <= songs; ++i)
    {
        NEZSetSongNo(m_input, i);
        NEZReset(m_input);

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, NEZGetTrackTitle(m_input, i));
            info->setValue(Qmmp::ARTIST, NEZGetGameArtist(m_input));
            info->setValue(Qmmp::ALBUM, NEZGetGameTitle(m_input));
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, NEZGetGameDetail(m_input));
        }

        info->setPath("xgm://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(NEZGetTrackLength(m_input, i));
        playlist << info;
    }
    return playlist;
}


class JaytraxReader : public AbstractReader
{
public:
    JaytraxReader(const QString &path);
    ~JaytraxReader();

    virtual bool load() override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;
    virtual QList<TrackInfo*> createPlayList(TrackInfo::Parts parts) override final;

private:
    JT1Song *m_song = nullptr;
    JT1Player *m_input = nullptr;

};

JaytraxReader::JaytraxReader(const QString &path)
    : AbstractReader(path)
{

}

JaytraxReader::~JaytraxReader()
{
    if(m_song)
    {
        jxsfile_freeSong(m_song);
    }

    if(m_input)
    {
        jaytrax_free(m_input);
    }
}

bool JaytraxReader::load()
{
    QFile file(cleanPath());
    if(!file.open(QIODevice::ReadOnly))
    {
//        qWarning("JaytraxReader: open file failed");
        return false;
    }

    const QByteArray &buffer = file.readAll();
    file.close();

    if(jxsfile_readSongMem((uint8_t*)buffer.constData(), buffer.length(), &m_song) != 0)
    {
//        qWarning("JaytraxReader: jxsfile_readSongMem failed");
        return false;
    }

    m_input = jaytrax_init();
    jaytrax_loadSong(m_input, m_song);

    int track = m_path.section("#", -1).toInt() - 1;
    track = track < 0 ? 0 : track;
    jaytrax_changeSubsong(m_input, track);
    m_length = jaytrax_getLength(m_input, track, 1, sampleRate()) / sampleRate() * 1000;
    m_totalSamples = totalTime() * sampleRate() / 1000;
    return true;
}

void JaytraxReader::seek(qint64 time)
{
    const int sample = time * sampleRate() / 1000;
    if(sample < m_currentSample)
    {
        JT1Song *oldSong = m_song;
        JT1Player *oldInput = m_input;
        //
        JaytraxReader::load();
        //
        jxsfile_freeSong(oldSong);
        jaytrax_free(oldInput);
        m_currentSample = 0;
    }

    if(m_currentSample != sample)
    {
        jaytrax_renderChunk(m_input, nullptr, sample - m_currentSample, sampleRate());
        m_currentSample = sample;
    }
}

qint64 JaytraxReader::read(unsigned char *data, qint64 maxSize)
{
    if(m_currentSample >= m_totalSamples)
    {
        return 0;
    }

    const int size = maxSize / (channels() * depth() / 8);
    m_currentSample += size;
    jaytrax_renderChunk(m_input, (int16_t*)data, size, sampleRate());
    return maxSize;
}

QList<TrackInfo*> JaytraxReader::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    const int songs = m_song->nrofsongs;
    for(int i = 1; i <= songs; ++i)
    {
        jaytrax_changeSubsong(m_input, i - 1);

        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, (*m_song->subsongs)->name);
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, "Jaytrax");
        }

        info->setPath("xgm://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(jaytrax_getLength(m_input, i - 1, 1, sampleRate()) / sampleRate() * 1000);
        playlist << info;
    }
    return playlist;
}


class PacReader : public AbstractReader
{
public:
    PacReader(const QString &path);
    ~PacReader();

    virtual bool load() override final;
    virtual qint64 read(unsigned char *data, qint64 maxSize) override final;
    virtual void seek(qint64 time) override final;
    virtual QList<TrackInfo*> createPlayList(TrackInfo::Parts parts) override final;

private:
    pac_module *m_input = nullptr;

};

PacReader::PacReader(const QString &path)
    : AbstractReader(path)
{

}

PacReader::~PacReader()
{
    if(m_input)
    {
        pac_close(m_input);
    }
}

bool PacReader::load()
{
    pac_init(sampleRate(), depth(), channels());

    m_input = pac_open(QmmpPrintable(cleanPath()));
    if(!m_input)
    {
//        qWarning("PacReader: pac_open failed");
        return false;
    }

    m_totalSamples = pac_length(m_input);
    m_length = m_totalSamples * 1000 / sampleRate();
    return true;
}

void PacReader::seek(qint64 time)
{
    pac_seek(m_input, (time / 1000) * sampleRate(), SEEK_SET);
}

qint64 PacReader::read(unsigned char *data, qint64 maxSize)
{
    return pac_read(m_input, data, maxSize / (channels() * depth() / 8));
}

QList<TrackInfo*> PacReader::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!m_input)
    {
        return playlist;
    }

    TrackInfo *info = new TrackInfo();
    if(parts & TrackInfo::MetaData)
    {
        info->setValue(Qmmp::TITLE, pac_title(m_input));
        info->setValue(Qmmp::TRACK, 1);
    }

    if(parts & TrackInfo::Properties)
    {
        info->setValue(Qmmp::BITRATE, bitrate());
        info->setValue(Qmmp::SAMPLERATE, sampleRate());
        info->setValue(Qmmp::CHANNELS, channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
        info->setValue(Qmmp::FORMAT_NAME, "SBStudio PAC");
    }

    info->setPath("xgm://" + cleanPath() + "#1");
    info->setDuration(m_length);
    playlist << info;
    return playlist;
}


XGMHelper::XGMHelper(const QString &path)
{
#define MAKE_GM_READER(T, path, d) \
    delete m_input; \
    m_input = new T(path); \
    if(m_input->load()) \
    { \
        qDebug("XGM: use " d " reader"); \
        return; \
    }

    MAKE_GM_READER(KssReader, path, "Kss");
    MAKE_GM_READER(NEZplugReader, path, "NEZplug++");
    MAKE_GM_READER(JaytraxReader, path, "Jaytrax");
    MAKE_GM_READER(PacReader, path, "SBStudio PAC");

#undef MAKE_GM_READER
    qDebug("XGM: no reader found");
    delete m_input;
    m_input = nullptr;
}

XGMHelper::~XGMHelper()
{
    deinit();
}

void XGMHelper::deinit()
{
    delete m_input;
}

bool XGMHelper::initialize()
{
    return m_input;
}
