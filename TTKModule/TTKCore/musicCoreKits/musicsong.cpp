#include "musicsong.h"
#include "musicstringutils.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicextractwrapper.h"
#include "musicsettingmanager.h"

MusicSong::MusicSong()
    : m_sortType(SortByFileName),
      m_musicSize(0),
      m_musicAddTime(-1),
      m_musicSizeStr(TTK_DEFAULT_STR),
      m_musicAddTimeStr(TTK_DEFAULT_STR),
      m_musicPlayCount(0),
      m_musicName(TTK_DEFAULT_STR),
      m_musicPath(TTK_DEFAULT_STR),
      m_musicType(TTK_DEFAULT_STR),
      m_musicPlayTime(TTK_DEFAULT_STR)
{

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

    const QFileInfo fin(m_musicPath);
    m_musicName = musicName;
    if(m_musicName.isEmpty())
    {
        m_musicName = fin.completeBaseName();
    }

    setMusicSize(fin.size());
    m_musicType = FILE_SUFFIX(fin);
    m_musicAddTime = fin.lastModified().currentMSecsSinceEpoch();
    m_musicPlayTime = playTime;
    m_musicAddTimeStr = QString::number(m_musicAddTime);
}

QString MusicSong::musicArtistFront() const
{
    return MusicUtils::String::artistName(m_musicName);
}

QString MusicSong::musicArtistBack() const
{
    return MusicUtils::String::songName(m_musicName);
}

void MusicSong::setMusicSize(const qint64 s)
{
    m_musicSize = s;
    m_musicSizeStr = MusicUtils::Number::sizeByte2Label(s);
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


MusicSongList MusicObject::generateMusicSongList(const QString &path)
{
    MusicSongList songs;
    const QStringList &support = MusicFormats::supportMusicFormats();
    const QString &suffix = FILE_SUFFIX(QFileInfo(path));

    if(!support.contains(suffix))
    {
        return songs;
    }

    MusicSongMeta meta;
    if(!meta.read(path))
    {
        return songs;
    }

    if(MusicFormats::songTrackTpyeContains(suffix))
    {
        const int size = meta.songMetaCount();
        for(int i=0; i<size; ++i)
        {
            meta.setSongMetaIndex(i);

            QString name;
            if(G_SETTING_PTR->value(MusicSettingManager::OtherUseFileInfo).toBool())
            {
                const QString &title = meta.title();
                const QString &artist = meta.artist();
                name = (artist.isEmpty() || title.isEmpty()) ? artist + title : artist + " - " + title;
            }

            const QFileInfo fin(meta.fileRelatedPath());
            MusicSong song(meta.fileBasePath(), meta.lengthString(), name);
            song.setMusicSize(fin.size());
            song.setMusicType(FILE_SUFFIX(fin));
            songs << song;
        }
    }
    else
    {
        QString name;
        if(G_SETTING_PTR->value(MusicSettingManager::OtherUseFileInfo).toBool())
        {
            const QString &title = meta.title();
            const QString &artist = meta.artist();
            name = (artist.isEmpty() || title.isEmpty()) ? artist + title : artist + " - " + title;
        }

        songs << MusicSong(path, meta.lengthString(), name);
    }
    return songs;
}

bool MusicObject::playlistRowValid(int index)
{
    return index != MUSIC_LOVEST_LIST && index != MUSIC_NETWORK_LIST && index != MUSIC_RECENT_LIST;
}
