#include "ttkmusicplaylist.h"
#include "musictime.h"

TTKMusicPlaylist::TTKMusicPlaylist(QObject *parent)
    : QObject(parent)
{
    MusicTime::timeSRand();
    m_currentIndex = -1;
    m_playbackMode = MusicObject::MC_PlayRandom;
}

int TTKMusicPlaylist::playbackMode() const
{
    return m_playbackMode;
}

void TTKMusicPlaylist::setPlaybackMode(int mode)
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

void TTKMusicPlaylist::removeMedia(const QString &content)
{
    m_mediaList.removeOne(content);
}

void TTKMusicPlaylist::removeMedia(int index)
{
    if(index < 0 || index >= m_mediaList.count())
    {
        return;
    }
    m_mediaList.removeAt(index);
}

QString TTKMusicPlaylist::currentMediaString() const
{
    if(m_currentIndex == -1 || m_currentIndex >= m_mediaList.count())
    {
        return QString();
    }
    return m_mediaList.isEmpty() ? QString()
                                 : m_mediaList[m_currentIndex];
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
    if(isEmpty())
    {
        return;
    }
    if(playbackMode() == MusicObject::MC_PlayRandom)
    {
        setCurrentIndex();
    }
    else
    {
        int index = ++m_currentIndex;
        setCurrentIndex((index >= mediaCount()) ? 0 : index);
    }
}

void TTKMusicPlaylist::playPrevious()
{
    if(isEmpty())
    {
        return;
    }
    if(playbackMode() == MusicObject::MC_PlayRandom)
    {
        setCurrentIndex();
    }
    else
    {
        int index = --m_currentIndex;
        setCurrentIndex((index < 0) ? 0 : index);
    }
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
    m_mediaList << items;
}
