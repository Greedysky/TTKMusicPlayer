#include "musicsong.h"
#include "musicnumberutils.h"
#include "musicsongmeta.h"
#include "musicformats.h"
#include "musicsettingmanager.h"

MusicSong::MusicSong() noexcept
    : m_sort(Sort::ByFileName),
      m_size(0),
      m_addTime(-1),
      m_sizeStr(TTK_DEFAULT_STR),
      m_addTimeStr(TTK_DEFAULT_STR),
      m_playCount(0),
      m_name(TTK_DEFAULT_STR),
      m_path(TTK_DEFAULT_STR),
      m_format(TTK_DEFAULT_STR),
      m_duration(TTK_DEFAULT_STR)
{

}

MusicSong::MusicSong(const QString &path, bool track)
    : MusicSong(path, {}, {}, track)
{

}

MusicSong::MusicSong(const QString &path, const QString &duration, const QString &name, bool track)
    : MusicSong()
{
    m_path = path;
    // replace windows \\ path to / path
    m_path.replace(TTK_WSEPARATOR, TTK_SEPARATOR);

    const QFileInfo fin(!track ? m_path : TTK::trackRelatedPath(m_path));

    m_name = name.isEmpty() ? fin.completeBaseName() : name;
    m_size = fin.size();
    m_format = TTK_FILE_SUFFIX(fin);
    m_addTime = fin.lastModified().toMSecsSinceEpoch();
    m_duration = duration;
    m_addTimeStr = QString::number(m_addTime);
    m_sizeStr = TTK::Number::sizeByteToLabel(m_size);
}

QString MusicSong::title() const noexcept
{
    return TTK::generateSongTitle(m_name);
}

QString MusicSong::artist() const noexcept
{
    return TTK::generateSongArtist(m_name);
}

bool MusicSong::operator== (const MusicSong &other) const noexcept
{
    return m_path == other.m_path;
}

bool MusicSong::operator< (const MusicSong &other) const noexcept
{
    switch(m_sort)
    {
        case Sort::ByFileName: return m_name < other.m_name;
        case Sort::BySinger: return artist() < other.artist();
        case Sort::ByFileSize: return m_size < other.m_size;
        case Sort::ByAddTime: return m_addTime < other.m_addTime;
        case Sort::ByDuration: return m_duration < other.m_duration;
        case Sort::ByPlayCount: return m_playCount < other.m_playCount;
        default: break;
    }
    return false;
}

bool MusicSong::operator> (const MusicSong &other) const noexcept
{
    switch(m_sort)
    {
        case Sort::ByFileName: return m_name > other.m_name;
        case Sort::BySinger: return artist() > other.artist();
        case Sort::ByFileSize: return m_size > other.m_size;
        case Sort::ByAddTime: return m_addTime > other.m_addTime;
        case Sort::ByDuration: return m_duration > other.m_duration;
        case Sort::ByPlayCount: return m_playCount > other.m_playCount;
        default: break;
    }
    return false;
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
        songs << MusicSong(path, meta.duration(), name, MusicFormats::isTrack(path));
    }

    if(!songs.isEmpty() && !meta.lyrics().isEmpty())
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
