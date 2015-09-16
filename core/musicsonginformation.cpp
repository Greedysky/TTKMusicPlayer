#include "musicsonginformation.h"

MusicSong::MusicSong()
    : m_musicName(QString()), m_musicPath(QString())
{
    m_musicType = QString();
    m_musicSize = 0;
    m_musicPlayCount = 0;
}

MusicSong::MusicSong(const QString &musicName, const QString &musicPath)
    : m_musicName(musicName), m_musicPath(musicPath)
{
    m_musicType = QString();
    m_musicSize = 0;
    m_musicPlayCount = 0;
}

QString MusicSong::getMusicArtistFront() const
{
    return m_musicName.split('-').front().trimmed();
}

QString MusicSong::getMusicArtistBack() const
{
    return m_musicName.split('-').back().trimmed();
}

