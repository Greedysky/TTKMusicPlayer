#include "musicsong.h"

#include <QStringList>

MusicSong::MusicSong()
    : m_musicName(QString()), m_musicPath(QString())
{
    m_musicSize = 0;
    m_musicPlayCount = 0;
}

MusicSong::MusicSong(const QString &musicPath, const QString &musicName)
    : MusicSong()
{
    m_musicPath = musicPath;
    m_musicName = musicName;

    QString splitString = musicPath.split("/").last();
    if(m_musicName.isEmpty())
    {
        m_musicName = splitString.left(splitString.lastIndexOf('.'));
    }
    m_musicType = splitString.remove(0, splitString.lastIndexOf('.') + 1);
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

QString MusicSong::getMusicArtistFront() const
{
    return m_musicName.split('-').front().trimmed();
}

QString MusicSong::getMusicArtistBack() const
{
    return m_musicName.split('-').back().trimmed();
}
