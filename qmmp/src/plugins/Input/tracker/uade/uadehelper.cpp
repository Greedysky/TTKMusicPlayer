#include "uadehelper.h"

#include <thread>

static std::thread UadeThread;

extern "C" {
void uade_run_thread(void (*f)(void*), void* data)
{
    UadeThread = std::thread(f, data);
}

void uade_wait_thread()
{
    UadeThread.join();
}
}


UADEHelper::UADEHelper(QObject *parent)
    : QObject(parent)
{

}

UADEHelper::~UADEHelper()
{
    deinit();
}

void UADEHelper::deinit()
{
    if(m_state)
    {
        uade_stop(m_state);
        uade_cleanup_state(m_state);
    }
}

bool UADEHelper::initialize(const QString &path, bool store)
{
    if(store)
    {
        m_path = path;
    }

    const int track = path.section("#", -1).toInt();
    const QString &ipath = cleanPath(path);

    m_mutex.lock();
    if(m_state)
    {
        uade_stop(m_state);
        uade_cleanup_state(m_state);
    }

    struct uade_config* config = uade_new_config();
    uade_config_set_option(config, UC_ONE_SUBSONG, NULL);
    uade_config_set_option(config, UC_IGNORE_PLAYER_CHECK, NULL);
    uade_config_set_option(config, UC_FREQUENCY, "44100");
    uade_config_set_option(config, UC_BASE_DIR, qPrintable(Qmmp::ttkPluginPath() + "/config/uade"));

    m_state = uade_new_state(config);
    free(config);

    if(uade_play(QmmpPrintable(ipath), track - 1, m_state) != 1)
    {
        qWarning("UADEHelper: Unable to open file, %s", qPrintable(ipath));
        m_mutex.unlock();
        return false;
    }

    m_mutex.unlock();
    return true;
}

UADEHelper *UADEHelper::instance()
{
    static UADEHelper helper;
    return &helper;
}

void UADEHelper::seek(qint64 time)
{
    m_mutex.lock();
    uade_seek(UADE_SEEK_SONG_RELATIVE, time / 1000.0, 0, m_state);
    m_mutex.unlock();
}

qint64 UADEHelper::totalTime()
{
    m_mutex.lock();
    const struct uade_song_info *info = uade_get_song_info(m_state);
    m_mutex.unlock();
    if(!info)
    {
        return 0;
    }

    const int time = info->duration;
    const int max = info->subsongs.max - info->subsongs.min + 1;
    if(max <= 0)
    {
        return 0;
    }

    return (time <= 0 ? (info->modulebytes / 8 / 10 / max) : time) * 1000;
}

qint64 UADEHelper::read(unsigned char *data, qint64 maxSize)
{
    m_mutex.lock();
    const int size = uade_read(data, maxSize, m_state);
    m_mutex.unlock();
    return size;
}

QList<TrackInfo*> UADEHelper::createPlayList(const QString &path, TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    if(!initialize(path))
    {
        return playlist;
    }

    const struct uade_song_info *tag = uade_get_song_info(m_state);
    if(!tag)
    {
        return playlist;
    }

    const QString &title = QFileInfo(cleanPath(path)).baseName();
    for(int i = 1; i <= tag->subsongs.max + 1; ++i)
    {
        TrackInfo *info = new TrackInfo();
        if(parts & TrackInfo::MetaData)
        {
            info->setValue(Qmmp::TITLE, title);
            info->setValue(Qmmp::TRACK, i);
        }

        if(parts & TrackInfo::Properties)
        {
            info->setValue(Qmmp::BITRATE, bitrate());
            info->setValue(Qmmp::SAMPLERATE, sampleRate());
            info->setValue(Qmmp::CHANNELS, channels());
            info->setValue(Qmmp::BITS_PER_SAMPLE, depth());
            info->setValue(Qmmp::FORMAT_NAME, "UADE");
        }

        info->setPath("uade://" + cleanPath(path) + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }

    if(!m_path.isEmpty())
    {
        initialize(m_path);
        seek(0);
    }
    return playlist;
}

QString UADEHelper::cleanPath(const QString &path) const
{
    QString v = path;
    if(path.contains("://"))
    {
        v.remove("uade://");
        v.remove(RegularWrapper("#\\d+$"));
    }
    return v;
}
