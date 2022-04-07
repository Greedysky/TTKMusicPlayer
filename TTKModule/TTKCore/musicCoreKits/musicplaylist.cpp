#include "musicplaylist.h"
#include "musictime.h"

MusicPlaylist::MusicPlaylist(QObject *parent)
    : QObject(parent)
{
    MusicTime::initRandom();
    m_currentIndex = -1;
    m_playbackMode = MusicObject::PlayOrder;
}

MusicObject::PlayMode MusicPlaylist::playbackMode() const
{
    return m_playbackMode;
}

void MusicPlaylist::setPlaybackMode(MusicObject::PlayMode mode)
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
    if(item.m_toolIndex == MUSIC_NETWORK_LIST && /*Replace network url path to local path*/
      (item.m_path.startsWith(HTTP_PREFIX) || item.m_path.startsWith(HTTPS_PREFIX)))
    {
        const QString &id = item.m_path.section("#", -1);
        if(id == item.m_path)
        {
            return item.m_path;
        }

        const QString &cachePath = CACHE_DIR_FULL + id;
        return QFile::exists(cachePath) ? cachePath : item.m_path;
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

void MusicPlaylist::add(const MusicPlayItem &item)
{
    clear();
    m_mediaList << item;
}

void MusicPlaylist::add(const MusicPlayItemList &items)
{
    clear();
    m_mediaList = items;
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

void MusicPlaylist::append(const MusicPlayItem &item)
{
    m_mediaList << item;
}

void MusicPlaylist::append(const MusicPlayItemList &items)
{
    m_mediaList << items;
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
    if(index == DEFAULT_LOW_LEVEL)
    {
        switch(m_playbackMode)
        {
            case MusicObject::PlayOneLoop: break;
            case MusicObject::PlayOrder:
            {
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = -1;
                }
                break;
            }
            case MusicObject::PlaylistLoop:
            {
                if(++m_currentIndex >= m_mediaList.count())
                {
                    m_currentIndex = 0;
                }
                break;
            }
            case MusicObject::PlayRandom:
            {
                m_currentIndex = MusicTime::random() % m_mediaList.count();
                break;
            }
            case MusicObject::PlayOnce: break;
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
