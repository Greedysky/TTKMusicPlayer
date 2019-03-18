#include "musicsong.h"
#include "musicstringutils.h"
#include "musicnumberutils.h"

#include <QFileInfo>
#include <QDateTime>
#include <QStringList>

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
