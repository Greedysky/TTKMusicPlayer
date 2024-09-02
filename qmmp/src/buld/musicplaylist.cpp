#include "musicplaylist.h"
#include "time.h"

MusicPlaylist::MusicPlaylist(QObject *parent)
    : QObject(parent)
{
    srand(time(0));
    m_currentIndex = 0;
    m_playbackMode = PlayOrder;
}

SongPlayType MusicPlaylist::playbackMode() const
{
    return m_playbackMode;
}

void MusicPlaylist::setPlaybackMode(SongPlayType mode)
{
    m_playbackMode = mode;
}

int MusicPlaylist::currentIndex() const
{
    return m_currentIndex;
}

QString MusicPlaylist::currentMediaString() const
{
    if(m_currentIndex == -1 || m_currentIndex >= m_mediaList.count())
    {
        return QString();
    }
    return m_mediaList.isEmpty() ? QString()
                                 : m_mediaList[m_currentIndex];
}

int MusicPlaylist::mediaCount() const
{
    return m_mediaList.count();
}

bool MusicPlaylist::isEmpty() const
{
    return m_mediaList.isEmpty();
}

bool MusicPlaylist::clear()
{
    m_mediaList.clear();
    return isEmpty() ? true : false;
}

void MusicPlaylist::updateMediaLists(const QStringList &list, int index)
{
    addMedia(list);
    setCurrentIndex(index);
}

void MusicPlaylist::addMedia(const QString &content)
{
    m_mediaList = QStringList(content);
}

void MusicPlaylist::addMedia(const QStringList &items)
{
    m_mediaList = items;
}

void MusicPlaylist::appendMedia(const QString &content)
{
    m_mediaList.append(content);
}

void MusicPlaylist::appendMedia(const QStringList &items)
{
    m_mediaList.append(items);
}

bool MusicPlaylist::insertMedia(int index, const QString &content)
{
    if(index < 0 || index > m_mediaList.count())
    {
        return false;
    }
    m_mediaList.insert(index,content);
    return true;
}

bool MusicPlaylist::insertMedia(int index, const QStringList &items)
{
    if(index < 0 || index > m_mediaList.count())
    {
        return false;
    }
    for(int i = 0; i < items.count(); ++i)
    {
        m_mediaList.insert(m_mediaList.count() + i, items[i]);
    }
    return true;
}

bool MusicPlaylist::removeMedia(int pos)
{
    if(pos < 0 || pos >= m_mediaList.count())
    {
        return false;
    }
    m_mediaList.removeAt(pos);
    emit removeCurrentMedia();
    return true;
}

bool MusicPlaylist::removeMedia(int start, int end)
{
    if(start > end || (start < 0 || end >= m_mediaList.count()))
    {
        return false;
    }

    for(int i = 0; i < end - start; ++i)
    {
        m_mediaList.removeAt(start);
    }
    return true;
}

void MusicPlaylist::setCurrentIndex(int index)
{
    if(index == -2)
    {
        switch(m_playbackMode)
        {
            case PlayOneLoop: break;
            case PlayOrder:
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = -1;
                }
                break;
            case PlayListLoop:
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = 0;
                }
                break;
            case PlayRandom:
                m_currentIndex = rand() % m_mediaList.count();
                break;
            case PlayOnce :
                break;
        }
    }
    else
    {
        m_currentIndex = index;
    }
    emit currentIndexChanged(m_currentIndex);
}
