#include "musicsong.h"
#include "musicstringutils.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicextractwrapper.h"
#include "musicsettingmanager.h"

#include <qmmp/regularexpression.h>

MusicSong::MusicSong()
    : m_sort(Sort::ByFileName),
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

    const QFileInfo fin(!track ? m_path : TTK::trackRelatedPath(m_path));
    m_name = name;
    if(m_name.isEmpty())
    {
        m_name = fin.completeBaseName();
    }

    m_size = fin.size();
    m_format = FILE_SUFFIX(fin);
    m_addTime = fin.lastModified().toMSecsSinceEpoch();
    m_playTime = playTime;
    m_addTimeStr = QString::number(m_addTime);
    m_sizeStr = TTK::Number::sizeByteToLabel(m_size);
}

QString MusicSong::artistFront() const
{
    return TTK::String::artistName(m_name);
}

QString MusicSong::artistBack() const
{
    return TTK::String::songName(m_name);
}

bool MusicSong::operator== (const MusicSong &other) const
{
    return m_path == other.m_path;
}

bool MusicSong::operator< (const MusicSong &other) const
{
    switch(m_sort)
    {
        case Sort::ByFileName: return m_name < other.m_name;
        case Sort::BySinger: return artistFront() < other.artistFront();
        case Sort::ByFileSize: return m_size < other.m_size;
        case Sort::ByAddTime: return m_addTime < other.m_addTime;
        case Sort::ByPlayTime: return m_playTime < other.m_playTime;
        case Sort::ByPlayCount: return m_playCount < other.m_playCount;
        default: break;
    }
    return false;
}

bool MusicSong::operator> (const MusicSong &other) const
{
    switch(m_sort)
    {
        case Sort::ByFileName: return m_name > other.m_name;
        case Sort::BySinger: return artistFront() > other.artistFront();
        case Sort::ByFileSize: return m_size > other.m_size;
        case Sort::ByAddTime: return m_addTime > other.m_addTime;
        case Sort::ByPlayTime: return m_playTime > other.m_playTime;
        case Sort::ByPlayCount: return m_playCount > other.m_playCount;
        default: break;
    }
    return false;
}


bool TTK::playlistRowValid(int index)
{
    return index != MUSIC_LOVEST_LIST && index != MUSIC_NETWORK_LIST && index != MUSIC_RECENT_LIST;
}

QString TTK::trackRelatedPath(const QString &path)
{
    if(!MusicFormats::songTrackValid(path))
    {
        return path;
    }

    QString url = path.section("://", -1);
    url.remove(RegularExpression("#\\d+$"));
    return url;
}

QString TTK::generateSongName(const QString &title, const QString &artist)
{
    return (title.isEmpty() || artist.isEmpty()) ? artist + title : artist + " - " + title;
}

MusicSongList TTK::generateSongList(const QString &path)
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
            name = TTK::generateSongName(meta.title(), meta.artist());
        }

        const QString &path = meta.fileBasePath();
        songs << MusicSong(path, meta.duration(), name, MusicFormats::songTrackValid(path));
    }

    if(!(songs.isEmpty() || meta.lyrics().isEmpty()))
    {
        QFile file(TTK::String::lrcDirPrefix() + songs.back().name() + LRC_FILE);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(meta.lyrics().toUtf8());
            file.close();
        }
    }
    return songs;
}

QString TTK::generateNetworkSongTime(const QString &path)
{
    MusicSongMeta meta;
    return meta.read(TTK::generateNetworkSongPath(path)) ? meta.duration() : TTK_DEFAULT_STR;
}

QString TTK::generateNetworkSongPath(const QString &path)
{
    QString v = path;
    if(TTK::String::isNetworkUrl(path))
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
    return v;
}
