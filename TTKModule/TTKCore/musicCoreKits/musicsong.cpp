#include "musicsong.h"
#include "musicstringutils.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicextractwrapper.h"
#include "musicsettingmanager.h"

MusicSong::MusicSong()
    : m_musicName(),
      m_musicPath()
{
    m_sortType = SortByFileName;
    m_musicSize = 0;
    m_musicAddTime = -1;
    m_musicPlayCount = 0;
}

MusicSong::MusicSong(const QString &musicPath)
    : MusicSong(musicPath, QString())
{

}

MusicSong::MusicSong(const QString &musicPath, const QString &playTime, const QString &musicName)
    : MusicSong()
{
    m_musicPath = musicPath;
    m_musicPath.replace("\\", TTK_SEPARATOR);

    const QFileInfo info(m_musicPath);
    m_musicName = musicName;
    if(m_musicName.isEmpty())
    {
        m_musicName = info.completeBaseName();
    }

    m_musicSize = info.size();
    m_musicType = info.suffix();
    m_musicAddTime = info.lastModified().currentMSecsSinceEpoch();
    m_musicPlayTime = playTime;
    m_musicAddTimeStr = QString::number(m_musicAddTime);
    m_musicSizeStr = MusicUtils::Number::sizeByte2Label(m_musicSize);
}

QString MusicSong::musicArtistFront() const
{
    return MusicUtils::String::artistName(m_musicName);
}

QString MusicSong::musicArtistBack() const
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
        case SortByFileName: return m_musicName < other.m_musicName;
        case SortBySinger: return musicArtistFront() < other.musicArtistFront();
        case SortByFileSize: return m_musicSize < other.m_musicSize;
        case SortByAddTime: return m_musicAddTime < other.m_musicAddTime;
        case SortByPlayTime: return m_musicPlayTime < other.m_musicPlayTime;
        case SortByPlayCount: return m_musicPlayCount < other.m_musicPlayCount;
        default: break;
    }
    return false;
}

bool MusicSong::operator> (const MusicSong &other) const
{
    switch(m_sortType)
    {
        case SortByFileName: return m_musicName > other.m_musicName;
        case SortBySinger: return musicArtistFront() > other.musicArtistFront();
        case SortByFileSize: return m_musicSize > other.m_musicSize;
        case SortByAddTime: return m_musicAddTime > other.m_musicAddTime;
        case SortByPlayTime: return m_musicPlayTime > other.m_musicPlayTime;
        case SortByPlayCount: return m_musicPlayCount > other.m_musicPlayCount;
        default: break;
    }
    return false;
}


MusicSongs MusicObject::generateMusicSongList(const QString &path)
{
    MusicSongs songs;
    const QStringList &support = MusicFormats::supportMusicFormats();
    const QString &suffix = QFileInfo(path).suffix().toLower();

    if(!support.contains(suffix))
    {
        return songs;
    }

    if(suffix == ZIP_FILE_PREFIX)
    {
        QStringList outputs;
        if(!MusicExtractWrapper::outputBinary(path, G_SETTING_PTR->value(MusicSettingManager::DownloadMusicDirPath).toString(), outputs))
        {
            TTK_LOGGER_ERROR("Extract zip input error");
        }

        for(const QString &path : qAsConst(outputs))
        {
            songs << generateMusicSongList(path);
        }
    }
    else if(MusicFormats::songTrackTpyeContains(suffix))
    {
        MusicSongMeta meta;
        if(!meta.read(path))
        {
            return songs;
        }

        const int size = meta.songMetaSize();
        for(int i=0; i<size; ++i)
        {
            meta.setSongMetaIndex(i);
            const QString &time = meta.lengthString();
            const QString &title = meta.title();
            const QString &artist = meta.artist();

            QString name;
            if(G_SETTING_PTR->value(MusicSettingManager::OtherUseFileInfo).toBool() && !title.isEmpty() && !artist.isEmpty())
            {
                name = artist + " - " + title;
            }

            const QFileInfo fin(meta.fileRelatedPath());
            MusicSong song(meta.fileBasePath(), time, name);
            song.setMusicType(fin.suffix());
            song.setMusicSize(fin.size());
            songs << song;
        }

        return songs;
    }

    MusicSongMeta meta;
    const bool state = meta.read(path);
    const QString &time = state ? meta.lengthString() : TTK_DEFAULT_STR;

    QString name;
    if(state && G_SETTING_PTR->value(MusicSettingManager::OtherUseFileInfo).toBool() && !meta.title().isEmpty() && !meta.artist().isEmpty())
    {
        name = meta.artist() + " - " + meta.title();
    }
    songs << MusicSong(path, time, name);
    return songs;
}

bool MusicObject::playlistRowValid(int index)
{
    return index != MUSIC_LOVEST_LIST && index != MUSIC_NETWORK_LIST && index != MUSIC_RECENT_LIST;
}
