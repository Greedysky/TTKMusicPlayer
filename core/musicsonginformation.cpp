#include "musicsonginformation.h"

QMusicSong::QMusicSong()
    : m_musicName(QString()), m_musicPath(QString())
{
    m_musicType = QString();
    m_musicSize = 0;
    m_musicPlayCount = 0;
}

QMusicSong::QMusicSong(const QString &musicName,
                       const QString &musicPath)
    : m_musicName(musicName), m_musicPath(musicPath)
{
    m_musicType = QString();
    m_musicSize = 0;
    m_musicPlayCount = 0;
}

QString QMusicSong::getMusicArtistFront() const
{
    return m_musicName.split('-').front().trimmed();
}

QString QMusicSong::getMusicArtistBack() const
{
    return m_musicName.split('-').back().trimmed();
}

