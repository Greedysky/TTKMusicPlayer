#include "musicplaylist.h"
#include "musicstringutils.h"
#include "musicsong.h"
#include "ttktime.h"

MusicPlaylist::Shuffle::Shuffle()
    : m_index(-1),
      m_enable(false)
{
}

void MusicPlaylist::Shuffle::setEnabled(bool enable)
{
    m_enable = enable;
}

bool MusicPlaylist::Shuffle::isEnabled() const
{
    return m_enable;
}

void MusicPlaylist::Shuffle::initialize(const MusicPlayItemList &items)
{
    if(m_enable && items.count() != m_data.count())
    {
        m_index = -1;
        m_data = items;
        std::shuffle(m_data.begin(), m_data.end(), std::default_random_engine(std::random_device()()));
    }
}

void MusicPlaylist::Shuffle::setCurrentIndex(const MusicPlayItem &item)
{
    m_index = m_data.indexOf(item);
}

MusicPlayItem MusicPlaylist::Shuffle::setCurrentIndex(int index)
{
    if(m_data.isEmpty())
    {
        return {};
    }

    if(index == TTK_LOW_LEVEL|| index == PLAY_NEXT_LEVEL)
    {
        if(++m_index >= m_data.count())
        {
            m_index = 0;
        }
    }
    else if(index == PLAY_PREVIOUS_LEVEL)
    {
        if(--m_index < 0)
        {
            m_index =  m_data.count() - 1;
        }
    }
    else
    {
        m_index = index;
    }
    return m_data[m_index];
}


MusicPlaylist::MusicPlaylist(QObject *parent)
    : QObject(parent),
      m_currentIndex(-1),
      m_playbackMode(TTK::PlayMode::Order)
{
    TTK::initRandom();
}

void MusicPlaylist::setShuffleMode(bool shuffle)
{
    m_shuffle.setEnabled(shuffle);
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
    if(item.m_playlistRow == MUSIC_NETWORK_LIST)
    {
        return TTK::generateNetworkSongPath(item.m_path);
    }
    return item.m_path;
}

const MusicPlayItemList& MusicPlaylist::mediaList() const
{
    return m_mediaList;
}

const MusicPlayItemList& MusicPlaylist::queueList() const
{
    return m_queueList;
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

static void updatePlayItems(const MusicPairItemList &indexs, MusicPlayItemList &items)
{
    for(MusicPlayItem &item : items)
    {
        for(const MusicPairItem &index : qAsConst(indexs))
        {
            if(item.m_playlistRow != index.first)
            {
                continue;
            }

            item.m_playlistRow = index.second;
            break;
        }
    }
}

void MusicPlaylist::update(const MusicPairItemList &indexs)
{
    updatePlayItems(indexs, m_mediaList);
    updatePlayItems(indexs, m_queueList);
}

int MusicPlaylist::find(const MusicPlayItem &item) const
{
    return m_mediaList.indexOf(item);
}

int MusicPlaylist::find(int playlistRow, const QString &content, int from)
{
    return m_mediaList.indexOf({playlistRow, content}, from);
}

void MusicPlaylist::add(int playlistRow, const QString &content)
{
    clear();
    m_mediaList << MusicPlayItem(playlistRow, content);
}

void MusicPlaylist::add(int playlistRow, const QStringList &items)
{
    clear();
    for(const QString &path : qAsConst(items))
    {
        m_mediaList << MusicPlayItem(playlistRow, path);
    }
}

void MusicPlaylist::append(int playlistRow, const QString &content)
{
    m_mediaList << MusicPlayItem(playlistRow, content);
}

void MusicPlaylist::append(int playlistRow, const QStringList &items)
{
    for(const QString &path : qAsConst(items))
    {
        m_mediaList << MusicPlayItem(playlistRow, path);
    }
}

void MusicPlaylist::appendQueue(int playlistRow, const QString &content)
{
    if(m_currentIndex == -1)
    {
        return;
    }

    const int index = m_currentIndex + 1;
    (index != m_mediaList.count()) ? m_mediaList.insert(index, {playlistRow, content})
                                   : m_mediaList.append({playlistRow, content});
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

int MusicPlaylist::remove(int playlistRow, const QString &content)
{
    const int index = find(playlistRow, content);
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
    m_shuffle.initialize(m_mediaList);

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
                m_currentIndex = m_shuffle.isEnabled() ? find(m_shuffle.setCurrentIndex(index)) : (TTK::random() % m_mediaList.count());
                break;
            }
            case TTK::PlayMode::Once: break;
            default: break;
        }
    }
    else if(index == PLAY_NEXT_LEVEL)
    {
        if(m_playbackMode == TTK::PlayMode::Random)
        {
            m_currentIndex = m_shuffle.isEnabled() ? find(m_shuffle.setCurrentIndex(index)) : (TTK::random() % m_mediaList.count());
        }
        else
        {
            int index = m_currentIndex;
            m_currentIndex = (++index >= count()) ? 0 : index;
        }
    }
    else if(index == PLAY_PREVIOUS_LEVEL)
    {
        if(m_playbackMode == TTK::PlayMode::Random)
        {
            m_currentIndex = m_shuffle.isEnabled() ? find(m_shuffle.setCurrentIndex(index)) : (TTK::random() % m_mediaList.count());
        }
        else
        {
            int index = m_currentIndex;
            m_currentIndex = (--index < 0) ? 0 : index;
        }

        m_queueList.clear();
    }
    else
    {
        m_currentIndex = index;
        //
        if(m_playbackMode == TTK::PlayMode::Random && m_shuffle.isEnabled())
        {
            m_shuffle.setCurrentIndex(currentItem());
        }
    }

    if(!m_queueList.isEmpty())
    {
        const MusicPlayItem &item = m_queueList.takeFirst();
        m_currentIndex = item.m_playlistRow;
        if(m_currentIndex < 0 || m_currentIndex >= m_mediaList.count())
        {
            m_currentIndex = -1;
        }
    }

    Q_EMIT currentIndexChanged(m_currentIndex);
}

void MusicPlaylist::setCurrentIndex(int playlistRow, const QString &path)
{
    const int playIndex = find({playlistRow, path});
    setCurrentIndex(playIndex);
}
