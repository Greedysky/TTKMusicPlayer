#include "musicsong.h"
#include "musicstringutils.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicextractwrapper.h"
#include "musicsettingmanager.h"

#include <qmmp/regularwrapper.h>

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

MusicSong::MusicSong(const QString &musicPath, bool track)
    : MusicSong(musicPath, QString(), QString(), track)
{

}

MusicSong::MusicSong(const QString &musicPath, const QString &playTime, const QString &musicName, bool track)
    : MusicSong()
{
    m_musicPath = musicPath;
    m_musicPath.replace("\\", TTK_SEPARATOR);

    const QFileInfo fin(!track ? m_musicPath : MusicObject::trackRelatedPath(m_musicPath));
    m_musicName = musicName;
    if(m_musicName.isEmpty())
    {
        m_musicName = fin.completeBaseName();
    }

    m_musicSize = fin.size();
    m_musicType = FILE_SUFFIX(fin);
    m_musicAddTime = fin.lastModified().currentMSecsSinceEpoch();
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

QString MusicObject::trackRelatedPath(const QString &path)
{
    if(!MusicFormats::songTrackValid(path))
    {
        return path;
    }

    QString url = path;
    url = url.section("://", -1);
    url.remove(RegularWrapper("#\\d+$"));
    return url;
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

    const int size = meta.songMetaCount();
    for(int i = 0; i < size; ++i)
    {
        meta.setSongMetaIndex(i);

        QString name;
        if(G_SETTING_PTR->value(MusicSettingManager::OtherUseFileInfo).toBool())
        {
            const QString &title = meta.title();
            const QString &artist = meta.artist();
            name = (artist.isEmpty() || title.isEmpty()) ? artist + title : artist + " - " + title;
        }

        const QString &path = meta.fileBasePath();
        songs << MusicSong(path, meta.lengthString(), name, MusicFormats::songTrackValid(path));
    }
    return songs;
}

bool MusicObject::playlistRowValid(int index)
{
    return index != MUSIC_LOVEST_LIST && index != MUSIC_NETWORK_LIST && index != MUSIC_RECENT_LIST;
}
