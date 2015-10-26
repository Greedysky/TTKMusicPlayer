#include "musicsonginformation.h"
#include <QFileInfo>

MusicSong::MusicSong()
    : m_musicName(QString()), m_musicPath(QString())
{
    m_musicType = QString();
    m_musicSize = QFileInfo(m_musicPath).size();
    m_musicPlayCount = 0;
}

MusicSong::MusicSong(const QString &musicPath, const QString &musicName)
    : MusicSong()
{
    m_musicPath = musicPath;
    m_musicName = musicName;
    if(m_musicName.isEmpty())
    {
        QString splitString = musicPath.split("/").last();
        m_musicName = splitString.left(splitString.lastIndexOf('.'));
    }
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

QString MusicSong::getMusicArtistFront() const
{
    return m_musicName.split('-').front().trimmed();
}

QString MusicSong::getMusicArtistBack() const
{
    return m_musicName.split('-').back().trimmed();
}

