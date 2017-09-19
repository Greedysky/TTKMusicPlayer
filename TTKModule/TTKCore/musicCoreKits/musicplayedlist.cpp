#include "musicplayedlist.h"
#include "musictime.h"

MusicPlayedlist::MusicPlayedlist(QObject *parent)
    : QObject(parent)
{
    MusicTime::timeSRand();
    m_currentIndex = -1;
    m_playbackMode = MusicObject::PM_PlayOrder;
}

QString MusicPlayedlist::getClassName()
{
    return staticMetaObject.className();
}

MusicObject::PlayMode MusicPlayedlist::playbackMode() const
{
    return m_playbackMode;
}

void MusicPlayedlist::setPlaybackMode(MusicObject::PlayMode mode)
{
    m_playbackMode = mode;
}

int MusicPlayedlist::currentIndex() const
{
    return m_currentIndex;
}

MusicPlayedItem MusicPlayedlist::currentItem() const
{
    if(m_currentIndex == -1 || m_currentIndex >= m_mediaList.count())
    {
        return MusicPlayedItem();
    }
    return m_mediaList.isEmpty() ? MusicPlayedItem() : m_mediaList[m_currentIndex];
}

QString MusicPlayedlist::currentMediaString() const
{
    return currentItem().m_path;
}

MusicPlayedItems MusicPlayedlist::mediaListConst() const
{
    return m_mediaList;
}

MusicPlayedItems *MusicPlayedlist::mediaList()
{
    return &m_mediaList;
}

int MusicPlayedlist::mediaCount() const
{
    return m_mediaList.count();
}

bool MusicPlayedlist::isEmpty() const
{
    return m_mediaList.isEmpty();
}

bool MusicPlayedlist::clear()
{
    m_mediaList.clear();
    return isEmpty();
}

int MusicPlayedlist::find(int toolIndex, const QString &content, int from)
{
    return m_mediaList.indexOf(MusicPlayedItem(toolIndex, content), from);
}

void MusicPlayedlist::addMedia(int toolIndex, const QString &content)
{
    m_mediaList.clear();
    m_mediaList << MusicPlayedItem(toolIndex, content);
}

void MusicPlayedlist::addMedia(int toolIndex, const QStringList &items)
{
    m_mediaList.clear();
    foreach(const QString &path, items)
    {
        m_mediaList << MusicPlayedItem(toolIndex, path);
    }
}

void MusicPlayedlist::addMedia(const MusicPlayedItem &item)
{
    m_mediaList.clear();
    m_mediaList << item;
}

void MusicPlayedlist::addMedia(const MusicPlayedItems &items)
{
    m_mediaList = items;
}

void MusicPlayedlist::appendMedia(int toolIndex, const QString &content)
{
    m_mediaList << MusicPlayedItem(toolIndex, content);
}

void MusicPlayedlist::appendMedia(int toolIndex, const QStringList &items)
{
    foreach(const QString &path, items)
    {
        m_mediaList << MusicPlayedItem(toolIndex, path);
    }
}

void MusicPlayedlist::appendMedia(const MusicPlayedItem &item)
{
    m_mediaList << item;
}

void MusicPlayedlist::appendMedia(const MusicPlayedItems &items)
{
    m_mediaList << items;
}

void MusicPlayedlist::insertLaterMedia(int toolIndex, const QString &content)
{
    if(m_currentIndex != 0)
    {
        m_mediaList.insert(m_currentIndex, MusicPlayedItem(toolIndex, content));
    }
}

bool MusicPlayedlist::removeMedia(int pos)
{
    if( pos < 0 || pos >= m_mediaList.count())
    {
        return false;
    }
    m_mediaList.removeAt(pos);
    return true;
}

bool MusicPlayedlist::removeMedia(int start, int end)
{
    if( start > end || (start < 0 || end >= m_mediaList.count()) )
    {
        return false;
    }
    for(int i=0; i<end - start; ++i)
    {
        m_mediaList.removeAt(start);
    }
    return true;
}

int MusicPlayedlist::removeMedia(int toolIndex, const QString &content)
{
    int index = find(toolIndex, content);
    if(index != -1)
    {
        m_mediaList.takeAt(index);
    }

    return index;
}

void MusicPlayedlist::setCurrentIndex(int index)
{
    if(index == DEFAULT_INDEX_LEVEL1)
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
            case MusicObject::PM_PlayListLoop:
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
        }
    }
    else
    {
        m_currentIndex = index;
    }

    emit currentIndexChanged(m_currentIndex);
}

void MusicPlayedlist::setCurrentIndex(int toolIndex, const QString &path)
{
    int playIndex = -1;
    for(int i=0; i<m_mediaList.count(); ++i)
    {
        const MusicPlayedItem &item = m_mediaList[i];
        if(item.m_toolIndex == toolIndex && item.m_path == path)
        {
            playIndex = i;
            break;
        }
    }

    setCurrentIndex(playIndex);
}
