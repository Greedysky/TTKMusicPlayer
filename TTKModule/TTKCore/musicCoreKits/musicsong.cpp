#include "musicsong.h"
#include "musicstringutils.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicextractwrap.h"
#include "musicsettingmanager.h"

MusicSong::MusicSong()
    : m_musicName(QString()), m_musicPath(QString())
{
    m_sortType = SortByFileName;
    m_musicSize = 0;
    m_musicAddTime = -1;
    m_musicPlayCount = 0;
}

MusicSong::MusicSong(const QString &musicPath, const QString &musicName)
    : MusicSong()
{
    m_musicPath = musicPath;
    m_musicName = musicName;

    m_musicPath.replace("\\", "/");

    const QFileInfo info(m_musicPath);
    if(m_musicName.isEmpty())
    {
        m_musicName = info.completeBaseName();
    }
    m_musicSize = info.size();
    m_musicType = info.suffix();
    m_musicAddTime = info.lastModified().currentMSecsSinceEpoch();
    m_musicAddTimeStr = QString::number(m_musicAddTime);
    m_musicSizeStr = MusicUtils::Number::size2Label(m_musicSize);
}

MusicSong::MusicSong(const QString &musicPath, int playCount, const QString &musicName)
    : MusicSong(musicPath, musicName)
{
    m_musicPlayCount = playCount;
}

MusicSong::MusicSong(const QString &musicPath, const QString &type, int playCount, const QString &musicName)
    : MusicSong(musicPath, playCount, musicName)
{
    m_musicType = type;
}

MusicSong::MusicSong(const QString &musicPath, const QString &type, const QString &playTime, int playCount, const QString &musicName)
    : MusicSong(musicPath, type, playCount, musicName)
{
    m_musicPlayTime = playTime;
}

MusicSong::MusicSong(const QString &musicPath, int playCount, const QString &time, const QString &musicName)
    : MusicSong(musicPath, playCount, musicName)
{
    m_musicPlayTime = time;
}

QString MusicSong::getMusicArtistFront() const
{
    return MusicUtils::String::artistName(m_musicName);
}

QString MusicSong::getMusicArtistBack() const
{
    return MusicUtils::String::songName(m_musicName);
}

bool MusicSong::operator== (const MusicSong &other) const
{
    return m_musicPath == other.m_musicPath;
}

bool MusicSong::operator< (const MusicSong &other) const
{
    switch(m_sortType)
    {
        case SortByFileName : return m_musicName < other.m_musicName;
        case SortBySinger : return getMusicArtistFront() < other.getMusicArtistFront();
        case SortByFileSize : return m_musicSize < other.m_musicSize;
        case SortByAddTime : return m_musicAddTime < other.m_musicAddTime;
        case SortByPlayTime : return m_musicPlayTime < other.m_musicPlayTime;
        case SortByPlayCount : return m_musicPlayCount < other.m_musicPlayCount;
        default: break;
    }
    return false;
}

bool MusicSong::operator> (const MusicSong &other) const
{
    switch(m_sortType)
    {
        case SortByFileName : return m_musicName > other.m_musicName;
        case SortBySinger : return getMusicArtistFront() > other.getMusicArtistFront();
        case SortByFileSize : return m_musicSize > other.m_musicSize;
        case SortByAddTime : return m_musicAddTime > other.m_musicAddTime;
        case SortByPlayTime : return m_musicPlayTime > other.m_musicPlayTime;
        case SortByPlayCount : return m_musicPlayCount > other.m_musicPlayCount;
        default: break;
    }
    return false;
}


MusicSongs MusicObject::generateMusicSong(const QString &path)
{
    MusicSongs songs;
    const QStringList &support = MusicFormats::supportFormatsString();
    const QString &suffix = QFileInfo(path).suffix().toLower();

    if(!support.contains(suffix))
    {
        return songs;
    }

    if(suffix == ZIP_FILE_PREFIX)
    {
        QStringList outputs;
        if(!MusicExtractWrap::outputBinary(path, M_SETTING_PTR->value(MusicSettingManager::DownloadMusicPathDir).toString(), outputs))
        {
            TTK_LOGGER_ERROR("Extract zip input error");
        }

        for(const QString &path : qAsConst(outputs))
        {
            songs << generateMusicSong(path);
        }
    }
    else if(suffix == CUE_FILE_PREFIX)
    {
        MusicSongMeta meta;
        if(!meta.read(path))
        {
            return songs;
        }

        for(const MusicSongMeta::MusicMeta &info : qAsConst(meta.getSongMetas()))
        {
            const QString &time = info.m_metaData[TagWrapper::TAG_LENGTH];
            const QString &title = info.m_metaData[TagWrapper::TAG_TITLE];
            const QString &artist = info.m_metaData[TagWrapper::TAG_ARTIST];

            QString name;
            if(M_SETTING_PTR->value(MusicSettingManager::OtherUseInfo).toBool() && !title.isEmpty() && !artist.isEmpty())
            {
                name = artist + " - " + title;
            }
            songs << MusicSong(info.m_filePath, 0, time, name);
        }

        return songs;
    }

    MusicSongMeta meta;
    const bool state = meta.read(path);
    const QString &time = state ? meta.getLengthString() : STRING_NULL;

    QString name;
    if(state && M_SETTING_PTR->value(MusicSettingManager::OtherUseInfo).toBool() && !meta.getTitle().isEmpty() && !meta.getArtist().isEmpty())
    {
        name = meta.getArtist() + " - " + meta.getTitle();
    }
    songs << MusicSong(path, 0, time, name);

    return songs;
}
