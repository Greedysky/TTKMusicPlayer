#include "uadehelper.h"

UADEHelper::UADEHelper(const QString &path)
    : m_path(path)
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
        m_tags.clear();
        uade_stop(m_state);
        uade_cleanup_state(m_state);
    }
}

bool UADEHelper::initialize()
{
    struct uade_config* config = uade_new_config();
    uade_config_set_option(config, UC_ONE_SUBSONG, NULL);
    uade_config_set_option(config, UC_IGNORE_PLAYER_CHECK, NULL);
    uade_config_set_option(config, UC_FREQUENCY, "44100");
    uade_config_set_option(config, UC_BASE_DIR, qPrintable(Qmmp::ttkPluginPath() + "/config/uade"));

    try
    {
        m_state = uade_new_state(config);
        free(config);
    }
    catch(...)
    {
        free(config);
        return false;
    }

    if(!m_state)
    {
        qWarning("UADEHelper: Unable to create uade state");
        return false;
    }

    m_track = m_path.section("#", -1).toInt() - 1;
    if(m_track < 0)
    {
        m_track = 0;
    }
    const QString &path = cleanPath();

    if(uade_play(QmmpPrintable(path), m_track, m_state) != 1)
    {
        qWarning("UADEHelper: Unable to open file, %s", qPrintable(path));
        return false;
    }

    const struct uade_song_info *info = uade_get_song_info(m_state);
    if(info)
    {
        m_tags.insert("modulemd5", info->modulemd5);
        m_tags.insert("modulepath", info->modulefname);
        m_tags.insert("formatname", info->formatname);
        m_tags.insert("modulename", info->modulename);
        m_tags.insert("playername", info->playername);
        m_tags.insert("format", info->detectioninfo.ext);
    }
    return true;
}

void UADEHelper::seek(qint64 time)
{
    uade_seek(UADE_SEEK_SONG_RELATIVE, time / 1000.0, m_track, m_state);
}

qint64 UADEHelper::totalTime()
{
    const struct uade_song_info *info = uade_get_song_info(m_state);
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
    return uade_read(data, maxSize, m_state);
}

QList<TrackInfo*> UADEHelper::createPlayList(TrackInfo::Parts parts)
{
    QList<TrackInfo*> playlist;
    const struct uade_song_info *tag = uade_get_song_info(m_state);
    if(!tag)
    {
        return playlist;
    }

    const QString &title = QFileInfo(cleanPath()).baseName();
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

        info->setPath("uade://" + cleanPath() + QString("#%1").arg(i));
        info->setDuration(totalTime());
        playlist << info;
    }
    return playlist;
}

QString UADEHelper::cleanPath() const
{
    QString path = m_path;
    if(m_path.contains("://"))
    {
        path.remove("uade://");
        path.remove(RegularWrapper("#\\d+$"));
    }
    return path;
}
