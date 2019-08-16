#include "musicplaylist.h"
#include "musictime.h"

MusicPlaylist::MusicPlaylist(QObject *parent)
    : QObject(parent)
{
    MusicTime::timeSRand();
    m_currentIndex = -1;
    m_playbackMode = MusicObject::PM_PlayOrder;
}

MusicObject::PlayMode MusicPlaylist::playbackMode() const
{
    return m_playbackMode;
}

void MusicPlaylist::setPlaybackMode(MusicObject::PlayMode mode)
{
    m_playbackMode = mode;
}

int MusicPlaylist::mapItemIndex(const MusicPlayItem &item) const
{
    int playIndex = -1;
    for(int i=0; i<m_mediaList.count(); ++i)
    {
        const MusicPlayItem &it = m_mediaList[i];
        if(item == it)
        {
            playIndex = i;
            break;
        }
    }

    return playIndex;
}

int MusicPlaylist::currentIndex() const
{
    return m_currentIndex;
}

MusicPlayItem MusicPlaylist::currentItem() const
{
    if(m_currentIndex == -1 || m_currentIndex >= m_mediaList.count())
    {
        return MusicPlayItem();
    }
    return m_mediaList.isEmpty() ? MusicPlayItem() : m_mediaList[m_currentIndex];
}

QString MusicPlaylist::currentMediaString() const
{
    return currentItem().m_path;
}

MusicPlayItems MusicPlaylist::mediaListConst() const
{
    return m_mediaList;
}

MusicPlayItems *MusicPlaylist::mediaList()
{
    return &m_mediaList;
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
    return isEmpty();
}

int MusicPlaylist::find(int toolIndex, const QString &content, int from)
{
    return m_mediaList.indexOf(MusicPlayItem(toolIndex, content), from);
}

void MusicPlaylist::addMedia(int toolIndex, const QString &content)
{
    m_mediaList.clear();
    m_laterMediaList.clear();
    m_mediaList << MusicPlayItem(toolIndex, content);
}

void MusicPlaylist::addMedia(int toolIndex, const QStringList &items)
{
    m_mediaList.clear();
    m_laterMediaList.clear();
    foreach(const QString &path, items)
    {
        m_mediaList << MusicPlayItem(toolIndex, path);
    }
}

void MusicPlaylist::addMedia(const MusicPlayItem &item)
{
    m_mediaList.clear();
    m_laterMediaList.clear();
    m_mediaList << item;
}

void MusicPlaylist::addMedia(const MusicPlayItems &items)
{
    m_laterMediaList.clear();
    m_mediaList = items;
}

void MusicPlaylist::appendMedia(int toolIndex, const QString &content)
{
    m_mediaList << MusicPlayItem(toolIndex, content);
}

void MusicPlaylist::appendMedia(int toolIndex, const QStringList &items)
{
    foreach(const QString &path, items)
    {
        m_mediaList << MusicPlayItem(toolIndex, path);
    }
}

void MusicPlaylist::appendMedia(const MusicPlayItem &item)
{
    m_mediaList << item;
}

void MusicPlaylist::appendMedia(const MusicPlayItems &items)
{
    m_mediaList << items;
}

MusicPlayItems MusicPlaylist::laterListConst() const
{
    return m_laterMediaList;
}

void MusicPlaylist::insertLaterMedia(int toolIndex, const QString &content)
{
    if(m_currentIndex != -1)
    {
        const int index = m_currentIndex + 1;
        (index != m_mediaList.count()) ? m_mediaList.insert(index, MusicPlayItem(toolIndex, content))
                                       : m_mediaList.append(MusicPlayItem(toolIndex, content));
        m_laterMediaList << MusicPlayItem(index + m_laterMediaList.count(), content);
    }
}

void MusicPlaylist::laterListClear()
{
    m_laterMediaList.clear();
}

bool MusicPlaylist::removeMedia(int pos)
{
    if(pos < 0 || pos >= m_mediaList.count())
    {
        return false;
    }

    m_mediaList.removeAt(pos);
    laterListClear();
    return true;
}

int MusicPlaylist::removeMedia(int toolIndex, const QString &content)
{
    const int index = find(toolIndex, content);
    if(index != -1)
    {
        m_mediaList.removeAt(index);
        laterListClear();
    }

    return index;
}

void MusicPlaylist::setCurrentIndex(int index)
{
    if(index == DEFAULT_LEVEL_NORMAL)
    {
        switch(m_playbackMode)
        {
            case MusicObject::PM_PlayOneLoop: break;
            case MusicObject::PM_PlayOrder:
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = -1;
                }
                break;
            case MusicObject::PM_PlaylistLoop:
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = 0;
                }
                break;
            case MusicObject::PM_PlayRandom:
                m_currentIndex = rand() % m_mediaList.count();
                break;
            case MusicObject::PM_PlayOnce :
                break;
            default: break;
        }
    }
    else
    {
        m_currentIndex = index;
    }

    if(!m_laterMediaList.isEmpty())
    {
        const MusicPlayItem &item = m_laterMediaList.takeFirst();
        m_currentIndex = item.m_toolIndex;
        if(m_currentIndex < 0 || m_currentIndex >= m_mediaList.count())
        {
            m_currentIndex = -1;
        }
    }

    emit currentIndexChanged(m_currentIndex);
}

void MusicPlaylist::setCurrentIndex(int toolIndex, const QString &path)
{
    const int playIndex = mapItemIndex(MusicPlayItem(toolIndex, path));
    setCurrentIndex(playIndex);
}
