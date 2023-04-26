#include "musicplaylist.h"
#include "musicstringutils.h"
#include "musicsong.h"
#include "ttktime.h"

MusicPlaylist::MusicPlaylist(QObject *parent)
    : QObject(parent),
      m_currentIndex(-1),
      m_playbackMode(TTK::PlayMode::Order)
{
    TTK::initRandom();
}

TTK::PlayMode MusicPlaylist::playbackMode() const
{
    return m_playbackMode;
}

void MusicPlaylist::setPlaybackMode(TTK::PlayMode mode)
{
    m_playbackMode = mode;
    Q_EMIT playbackModeChanged(m_playbackMode);
}

int MusicPlaylist::mapItemIndex(const MusicPlayItem &item) const
{
    int playIndex = -1;
    for(int i = 0; i < m_mediaList.count(); ++i)
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

QString MusicPlaylist::currentMediaPath() const
{
    const MusicPlayItem &item = currentItem();
    if(item.m_toolIndex == MUSIC_NETWORK_LIST)
    {
        return TTK::generateNetworkSongPath(item.m_path);
    }
    return item.m_path;
}

MusicPlayItemList *MusicPlaylist::mediaList()
{
    return &m_mediaList;
}

MusicPlayItemList *MusicPlaylist::queueList()
{
    return &m_queueList;
}

int MusicPlaylist::count() const
{
    return m_mediaList.count();
}

bool MusicPlaylist::isEmpty() const
{
    return m_mediaList.isEmpty();
}

void MusicPlaylist::clear()
{
    m_mediaList.clear();
    removeQueue();
}

int MusicPlaylist::find(int toolIndex, const QString &content, int from)
{
    return m_mediaList.indexOf({toolIndex, content}, from);
}

void MusicPlaylist::add(int toolIndex, const QString &content)
{
    clear();
    m_mediaList << MusicPlayItem(toolIndex, content);
}

void MusicPlaylist::add(int toolIndex, const QStringList &items)
{
    clear();
    for(const QString &path : qAsConst(items))
    {
        m_mediaList << MusicPlayItem(toolIndex, path);
    }
}

void MusicPlaylist::append(int toolIndex, const QString &content)
{
    m_mediaList << MusicPlayItem(toolIndex, content);
}

void MusicPlaylist::append(int toolIndex, const QStringList &items)
{
    for(const QString &path : qAsConst(items))
    {
        m_mediaList << MusicPlayItem(toolIndex, path);
    }
}

void MusicPlaylist::appendQueue(int toolIndex, const QString &content)
{
    if(m_currentIndex == -1)
    {
        return;
    }

    const int index = m_currentIndex + 1;
    (index != m_mediaList.count()) ? m_mediaList.insert(index, {toolIndex, content})
                                   : m_mediaList.append({toolIndex, content});
    m_queueList << MusicPlayItem(index + m_queueList.count(), content);
}

bool MusicPlaylist::remove(int pos)
{
    if(pos < 0 || pos >= m_mediaList.count())
    {
        return false;
    }

    m_mediaList.removeAt(pos);
    removeQueue();
    return true;
}

int MusicPlaylist::remove(int toolIndex, const QString &content)
{
    const int index = find(toolIndex, content);
    if(index != -1)
    {
        m_mediaList.removeAt(index);
        removeQueue();
    }

    return index;
}

void MusicPlaylist::removeQueue()
{
    m_queueList.clear();
}

void MusicPlaylist::setCurrentIndex(int index)
{
    if(index == TTK_LOW_LEVEL)
    {
        switch(m_playbackMode)
        {
            case TTK::PlayMode::OneLoop: break;
            case TTK::PlayMode::Order:
            {
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = -1;
                }
                break;
            }
            case TTK::PlayMode::ListLoop:
            {
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = 0;
                }
                break;
            }
            case TTK::PlayMode::Random:
            {
                m_currentIndex = TTK::random() % m_mediaList.count();
                break;
            }
            case TTK::PlayMode::Once: break;
            default: break;
        }
    }
    else
    {
        m_currentIndex = index;
    }

    if(!m_queueList.isEmpty())
    {
        const MusicPlayItem &item = m_queueList.takeFirst();
        m_currentIndex = item.m_toolIndex;
        if(m_currentIndex < 0 || m_currentIndex >= m_mediaList.count())
        {
            m_currentIndex = -1;
        }
    }

    Q_EMIT currentIndexChanged(m_currentIndex);
}

void MusicPlaylist::setCurrentIndex(int toolIndex, const QString &path)
{
    const int playIndex = mapItemIndex({toolIndex, path});
    setCurrentIndex(playIndex);
}
