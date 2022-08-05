#include "musicsong.h"
#include "musicstringutils.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicextractwrapper.h"
#include "musicsettingmanager.h"

#include <qmmp/regularwrapper.h>

MusicSong::MusicSong()
    : m_sort(SortByFileName),
      m_size(0),
      m_addTime(-1),
      m_sizeStr(TTK_DEFAULT_STR),
      m_addTimeStr(TTK_DEFAULT_STR),
      m_playCount(0),
      m_name(TTK_DEFAULT_STR),
      m_path(TTK_DEFAULT_STR),
      m_format(TTK_DEFAULT_STR),
      m_playTime(TTK_DEFAULT_STR)
{

}

MusicSong::MusicSong(const QString &path, bool track)
    : MusicSong(path, QString(), QString(), track)
{

}

MusicSong::MusicSong(const QString &path, const QString &playTime, const QString &name, bool track)
    : MusicSong()
{
    m_path = path;
    m_path.replace("\\", TTK_SEPARATOR);

    const QFileInfo fin(!track ? m_path : MusicObject::trackRelatedPath(m_path));
    m_name = name;
    if(m_name.isEmpty())
    {
        m_name = fin.completeBaseName();
    }

    m_size = fin.size();
    m_format = FILE_SUFFIX(fin);
    m_addTime = fin.lastModified().currentMSecsSinceEpoch();
    m_playTime = playTime;
    m_addTimeStr = QString::number(m_addTime);
    m_sizeStr = MusicUtils::Number::sizeByte2Label(m_size);
}

QString MusicSong::artistFront() const
{
    return MusicUtils::String::artistName(m_name);
}

QString MusicSong::artistBack() const
{
    return MusicUtils::String::songName(m_name);
}

bool MusicSong::operator== (const MusicSong &other) const
{
    return m_path == other.m_path;
}

bool MusicSong::operator< (const MusicSong &other) const
{
    switch(m_sort)
    {
        case SortByFileName: return m_name < other.m_name;
        case SortBySinger: return artistFront() < other.artistFront();
        case SortByFileSize: return m_size < other.m_size;
        case SortByAddTime: return m_addTime < other.m_addTime;
        case SortByPlayTime: return m_playTime < other.m_playTime;
        case SortByPlayCount: return m_playCount < other.m_playCount;
        default: break;
    }
    return false;
}

bool MusicSong::operator> (const MusicSong &other) const
{
    switch(m_sort)
    {
        case SortByFileName: return m_name > other.m_name;
        case SortBySinger: return artistFront() > other.artistFront();
        case SortByFileSize: return m_size > other.m_size;
        case SortByAddTime: return m_addTime > other.m_addTime;
        case SortByPlayTime: return m_playTime > other.m_playTime;
        case SortByPlayCount: return m_playCount > other.m_playCount;
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
        if(G_SETTING_PTR->value(MusicSettingManager::OtherReadFileInfo).toBool())
        {
            const QString &title = meta.title();
            const QString &artist = meta.artist();
            name = (artist.isEmpty() || title.isEmpty()) ? artist + title : artist + " - " + title;
        }

        const QString &path = meta.fileBasePath();
        songs << MusicSong(path, meta.duration(), name, MusicFormats::songTrackValid(path));
    }

    if(!(songs.isEmpty() || meta.lyrics().isEmpty()))
    {
        QFile file(MusicUtils::String::lrcDirPrefix() + songs.back().name() + LRC_FILE);
        if(file.open(QFile::WriteOnly))
        {
            file.write(meta.lyrics().toUtf8());
            file.close();
        }
    }
    return songs;
}

QString MusicObject::generateMusicPlayTime(const QString &path)
{
    QString v = path;
    if(path.startsWith(HTTP_PREFIX) || path.startsWith(HTTPS_PREFIX))
    {
        /*Replace network url path to local path*/
        const QString &id = path.section("#", -1);
        if(id != path)
        {
            const QString &cachePath = CACHE_DIR_FULL + id;
            if(QFile::exists(cachePath))
            {
                v = cachePath;
            }
        }
    }

    MusicSongMeta meta;
    return meta.read(v) ? meta.duration() : TTK_DEFAULT_STR;
}

bool MusicObject::playlistRowValid(int index)
{
    return index != MUSIC_LOVEST_LIST && index != MUSIC_NETWORK_LIST && index != MUSIC_RECENT_LIST;
}
