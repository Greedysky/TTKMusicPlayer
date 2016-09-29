#include "ttkmusicplaylist.h"
#include "musictime.h"

TTKMusicPlaylist::TTKMusicPlaylist(QObject *parent)
    : QObject(parent)
{
    MusicTime::timeSRand();
    m_currentIndex = -1;
    m_playbackMode = MusicObject::MC_PlayOrder;
}

MusicObject::SongPlayType TTKMusicPlaylist::playbackMode() const
{
    return m_playbackMode;
}

void TTKMusicPlaylist::setPlaybackMode(MusicObject::SongPlayType mode)
{
    m_playbackMode = mode;
}

void TTKMusicPlaylist::addMedia(const QString &content)
{
    addMedia(QStringList(content));
}

void TTKMusicPlaylist::addMedia(const QStringList &items)
{
    m_mediaList.clear();
    appendMedia(items);
}

QString TTKMusicPlaylist::media(int index) const
{
    if(index < 0 || index >= m_mediaList.count())
    {
        return QString();
    }
    return m_mediaList[index].getMusicPath();
}

QString TTKMusicPlaylist::mediaName(int index) const
{
    if(index < 0 || index >= m_mediaList.count())
    {
        return QString();
    }
    return m_mediaList[index].getMusicArtistBack();
}

QString TTKMusicPlaylist::mediaArtist(int index) const
{
    if(index < 0 || index >= m_mediaList.count())
    {
        return QString();
    }
    return m_mediaList[index].getMusicArtistFront();
}

int TTKMusicPlaylist::currentIndex() const
{
    return m_currentIndex;
}
int TTKMusicPlaylist::mediaCount() const
{
    return m_mediaList.count();
}

bool TTKMusicPlaylist::isEmpty() const
{
    return m_mediaList.isEmpty();
}

bool TTKMusicPlaylist::clear()
{
    m_mediaList.clear();
    return isEmpty();
}

void TTKMusicPlaylist::playNext()
{
    int index = ++m_currentIndex;
    setCurrentIndex((index >= mediaCount()) ? 0 : index);
}

void TTKMusicPlaylist::playPrevious()
{
    int index = --m_currentIndex;
    setCurrentIndex((index < 0) ? 0 : index);
}

void TTKMusicPlaylist::setCurrentIndex(int index)
{
    if(index == -2)
    {
        switch(m_playbackMode)
        {
            case MusicObject::MC_PlayOneLoop: break;
            case MusicObject::MC_PlayOrder:
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = -1;
                }
                break;
            case MusicObject::MC_PlayListLoop:
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = 0;
                }
                break;
            case MusicObject::MC_PlayRandom:
                m_currentIndex = rand() % m_mediaList.count();
                break;
            case MusicObject::MC_PlayOnce :
                break;
        }
    }
    else
    {
        m_currentIndex = index;
    }
    emit currentIndexChanged(m_currentIndex);
}

void TTKMusicPlaylist::appendMedia(const QString &content)
{
    appendMedia(QStringList(content));
}

void TTKMusicPlaylist::appendMedia(const QStringList &items)
{
    QStringList names, artist;
    foreach(const QString &item, items)
    {
        MusicSong song(item);
        names << song.getMusicArtistBack();
        artist << song.getMusicArtistFront();
        m_mediaList << song;
    }
    emit updateMedia(names, artist);
}
