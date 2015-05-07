#include "musicvideoplaylist.h"
#include "time.h"
#include <QDebug>

MusicVideoPlaylist::MusicVideoPlaylist(QObject *parent) :
    QObject(parent)
{
    qsrand(time(NULL));
    m_currentIndex = 0;
}

MusicVideoPlaylist::~MusicVideoPlaylist()
{

}

int MusicVideoPlaylist::currentIndex() const
{
    return m_currentIndex;
}

QString MusicVideoPlaylist::currentMediaString() const
{
    if(m_currentIndex == -1 || m_currentIndex >= m_mediaList.count())
        return QString();
    return m_mediaList.isEmpty() ? QString()
                                 : m_mediaList[m_currentIndex];
}

int MusicVideoPlaylist::mediaCount() const
{
    return m_mediaList.count();
}

bool MusicVideoPlaylist::isEmpty() const
{
    return m_mediaList.isEmpty();
}

bool MusicVideoPlaylist::clear()
{
    m_mediaList.clear();
    return isEmpty() ? true : false;
}

bool MusicVideoPlaylist::addMedia(const QString &content)
{
    m_mediaList = QStringList(content);
    return true;
}

bool MusicVideoPlaylist::addMedia(const QStringList &items)
{
    m_mediaList = items;
    return true;
}

void MusicVideoPlaylist::appendMedia(const QString &content)
{
    m_mediaList.append(content);
}

void MusicVideoPlaylist::appendMedia(const QStringList &items)
{
    m_mediaList.append(items);
}

bool MusicVideoPlaylist::insertMedia(int index, const QString &content)
{
    if( index < 0 || index > m_mediaList.count())
        return false;
    m_mediaList.insert(index,content);
    return true;
}

bool MusicVideoPlaylist::insertMedia(int index, const QStringList &items)
{
    if( index < 0 || index > m_mediaList.count())
        return false;
    for(int i = 0; i < items.count(); ++i)
        m_mediaList.insert(m_mediaList.count() + i, items[i]);
    return true;
}

bool MusicVideoPlaylist::removeMedia(int pos)
{
    if( pos < 0 || pos >= m_mediaList.count())
        return false;
    m_mediaList.removeAt(pos);
    emit removeCurrentMedia();
    return true;
}

bool MusicVideoPlaylist::removeMedia(int start, int end)
{
    if( start > end || (start < 0 || end >= m_mediaList.count()) )
        return false;
    for(int i=0; i<end - start; ++i)
        m_mediaList.removeAt(start);
    return true;
}

void MusicVideoPlaylist::setCurrentIndex(int index)
{
//    if( index < 0 || index >= m_mediaList.count())
//        return;
    m_currentIndex = index;
    emit currentIndexChanged(index);
}

void MusicVideoPlaylist::setCurrentIndex()
{
//    switch(m_playbackMode)
//    {
////        case MC_PlayOnce:break;
//        case MC_PlayOneLoop:break;
//        case MC_PlayOrder:
//            if(++m_currentIndex >= m_mediaList.count())
//                m_currentIndex = -1;
//            break;
//        case MC_PlayListLoop:
//            if(++m_currentIndex >= m_mediaList.count())
//                m_currentIndex = 0;
//            break;
//        case MC_PlayRandom:
//            m_currentIndex = rand() % m_mediaList.count();
//            break;
//    }
    emit currentIndexChanged(m_currentIndex);
}
