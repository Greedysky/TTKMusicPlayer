#include "musicsong.h"
#include "musiccoreutils.h"

#include <QFileInfo>
#include <QStringList>

MusicSong::MusicSong()
    : m_musicName(QString()), m_musicPath(QString())
{
    m_musicSize = 0;
    m_musicPlayCount = 0;
    m_toolIndex = -1;
}

MusicSong::MusicSong(const QString &musicPath, const QString &musicName)
    : MusicSong()
{
    m_musicPath = musicPath;
    m_musicName = musicName;

    QFileInfo info(m_musicPath);
    if(m_musicName.isEmpty())
    {
        m_musicName = info.completeBaseName();
    }
    m_musicType = info.suffix();
}

MusicSong::MusicSong(const QString &musicPath,
                     int playCount, const QString &musicName)
    : MusicSong(musicPath, musicName)
{
    m_musicPlayCount = playCount;
}

MusicSong::MusicSong(const QString &musicPath, const QString &type,
                     int playCount, const QString &musicName)
    : MusicSong(musicPath, playCount, musicName)
{
    m_musicType = type;
}

MusicSong::MusicSong(const QString &musicPath, const QString &type,
                     const QString &time, int playCount,
                     const QString &musicName)
    : MusicSong(musicPath, type, playCount, musicName)
{
    m_musicTime = time;
}

MusicSong::MusicSong(const QString &musicPath, int playCount, const QString &time,
                     const QString &musicName)
    : MusicSong(musicPath, playCount, musicName)
{
    m_musicTime = time;
}

QString MusicSong::getClassName()
{
    return "MusicSong";
}

QString MusicSong::getMusicArtistFront() const
{
    return MusicUtils::Core::artistName(m_musicName);
}

QString MusicSong::getMusicArtistBack() const
{
    return MusicUtils::Core::songName(m_musicName);
}

bool MusicSong::operator== (const MusicSong &other) const
{
    return m_musicPath == other.getMusicPath();
}
